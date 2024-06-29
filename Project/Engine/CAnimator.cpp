#include "pch.h"

#include "CAnimator.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CAssetMgr.h"
#include "CAnimationUpdateShader.h"
#include "CKeyMgr.h"

CAnimator::CAnimator()
    : CComponent(COMPONENT_TYPE::ANIMATOR)
    , m_SkeletalMesh(nullptr)
    , m_mapClip{}
    , m_CurClipIdx(0)
    , m_vecClipUpdateTime{}
    , m_bPlay(true)
    , m_bRepeat(true)
    , m_PlaySpeed(1.f)
    , m_FrameRate(30)
    , m_CurTime(0.)
    , m_FrameIdx(0)
    , m_NextFrameIdx(0)
    , m_Ratio(0.f)
    , m_BoneFinalMatBuffer(nullptr)
    , m_bFinalMatUpdate(false)
    , m_bChanging(false)
    , m_CurChangeTime(0.)
    , m_ChangeDuration(0.)
    , m_NextClipIdx(-1)
    , m_bNextRepeat(true)
    , m_NextPlaySpeed(1.f)
{
    m_BoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator::CAnimator(const CAnimator& _origin)
    : CComponent(_origin)
    , m_SkeletalMesh(_origin.m_SkeletalMesh)
    , m_mapClip(_origin.m_mapClip)
    , m_CurClipIdx(_origin.m_CurClipIdx)
    , m_vecClipUpdateTime(_origin.m_vecClipUpdateTime)
    , m_bPlay(_origin.m_bPlay)
    , m_bRepeat(_origin.m_bRepeat)
    , m_PlaySpeed(_origin.m_PlaySpeed)
    , m_FrameRate(_origin.m_FrameRate)
    , m_CurTime(_origin.m_CurTime)
    , m_FrameIdx(_origin.m_FrameIdx)
    , m_NextFrameIdx(_origin.m_NextFrameIdx)
    , m_Ratio(_origin.m_Ratio)
    , m_BoneFinalMatBuffer(nullptr)
    , m_bFinalMatUpdate(false)
    , m_bChanging(_origin.m_bChanging)
    , m_CurChangeTime(_origin.m_CurChangeTime)
    , m_ChangeDuration(_origin.m_ChangeDuration)
    , m_NextClipIdx(_origin.m_NextClipIdx)
    , m_bNextRepeat(_origin.m_bNextRepeat)
    , m_NextPlaySpeed(_origin.m_NextPlaySpeed)
{
    m_BoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator::~CAnimator()
{
    if (nullptr != m_BoneFinalMatBuffer)
    {
        delete m_BoneFinalMatBuffer;
        m_BoneFinalMatBuffer = nullptr;
    }
}

void CAnimator::finaltick()
{
    if (!IsVaild())
        return;

    // 다른 Clip 으로 전환 중인 경우
    if (m_bChanging)
    {
        if (m_bPlay)
        {
            m_CurChangeTime += DT * m_PlaySpeed;
        }

        m_Ratio = float(m_CurChangeTime / m_ChangeDuration);

        if (m_CurChangeTime >= m_ChangeDuration)
        {
            SetCurClipIdx(m_NextClipIdx);
            m_vecClipUpdateTime[m_CurClipIdx] = 0.f; // 애니메이션의 처음부터 시작

            // 애니메이션 재생 설정
            m_bRepeat = m_bNextRepeat;
            m_PlaySpeed = m_NextPlaySpeed;

            // 초기화
            m_bChanging = false;
            m_CurChangeTime = 0.;
            m_ChangeDuration = 0.;
            m_NextClipIdx = -1;
            m_bNextRepeat = true;
            m_NextPlaySpeed = 1.f;

            // 이번 프레임 애니메이션 설정
            m_FrameIdx = m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).iStartFrame;
            m_NextFrameIdx = m_FrameIdx + 1;
            m_Ratio = 0.;
        }
    }
    else
    {
        // 현재 재생중인 Clip 의 시간을 진행한다.
        if (m_bPlay)
        {
            m_vecClipUpdateTime[m_CurClipIdx] += DT * m_PlaySpeed;
        }

        if (m_vecClipUpdateTime[m_CurClipIdx] >= m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dTimeLength)
        {
            // 반복 재생
            if (m_bRepeat)
            {
                m_vecClipUpdateTime[m_CurClipIdx] = 0.f;
            }
            else
            {
                m_vecClipUpdateTime[m_CurClipIdx] = m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dTimeLength;
            }
        }

        m_CurTime = m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dStartTime + m_vecClipUpdateTime[m_CurClipIdx];

        // 현재 프레임 인덱스 구하기
        double dFrameIdx = m_CurTime * m_FrameRate;
        m_FrameIdx = (int)dFrameIdx;

        if (m_FrameIdx >= m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).iEndFrame)
            m_NextFrameIdx = m_FrameIdx; // 끝이면 현재 인덱스를 유지
        else
            m_NextFrameIdx = m_FrameIdx + 1;

        // 프레임간의 시간에 따른 비율을 구해준다.
        m_Ratio = (float)(dFrameIdx - (double)m_FrameIdx);
    }

    // 컴퓨트 쉐이더 연산여부
    m_bFinalMatUpdate = false;
}

void CAnimator::UpdateData()
{
    if (!IsVaild())
        return;

    if (!m_bFinalMatUpdate)
    {
        // Animation Update Compute Shader
        static CAnimationUpdateShader* pUpdateShader =
            (CAnimationUpdateShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"AnimationUpdateCS").Get();

        // Bone Data
        CheckBoneFinalMatBuffer();

        pUpdateShader->SetFrameDataBuffer(m_SkeletalMesh->GetBoneFrameDataBuffer());
        pUpdateShader->SetOffsetMatBuffer(m_SkeletalMesh->GetBoneOffsetBuffer());
        pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

        pUpdateShader->SetBoneCount(m_SkeletalMesh->GetBoneCount());
        pUpdateShader->SetFrameIndex(m_FrameIdx);
        pUpdateShader->SetNextFrameIdx(m_NextFrameIdx);
        pUpdateShader->SetFrameRatio(m_Ratio);

        // 업데이트 쉐이더 실행
        pUpdateShader->Execute();

        m_bFinalMatUpdate = true;
    }

    // t31 레지스터에 최종행렬 데이터(구조버퍼) 바인딩
    m_BoneFinalMatBuffer->UpdateData(31);
}

void CAnimator::finaltick_ModelEditor()
{
    if (!IsVaild())
        return;

    // 현재 재생중인 Clip 의 시간을 진행한다.
    if (m_bPlay)
    {
        m_vecClipUpdateTime[m_CurClipIdx] += DT_ENGINE * m_PlaySpeed;
    }

    if (m_vecClipUpdateTime[m_CurClipIdx] >= m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dTimeLength)
    {
        // 반복 재생
        if (m_bRepeat)
        {
            m_vecClipUpdateTime[m_CurClipIdx] = 0.f;
        }
        else
        {
            m_vecClipUpdateTime[m_CurClipIdx] = m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dTimeLength;
        }
    }

    m_CurTime = m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dStartTime + m_vecClipUpdateTime[m_CurClipIdx];

    // 현재 프레임 인덱스 구하기
    double dFrameIdx = m_CurTime * m_FrameRate;
    m_FrameIdx = (int)(dFrameIdx);

    // 다음 프레임 인덱스
    if (m_FrameIdx >= m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).iEndFrame)
        m_NextFrameIdx = m_FrameIdx; // 끝이면 현재 인덱스를 유지
    else
        m_NextFrameIdx = m_FrameIdx + 1;

    // 프레임간의 시간에 따른 비율을 구해준다.
    m_Ratio = (float)(dFrameIdx - (double)m_FrameIdx);

    // Animation Update Compute Shader
    static CAnimationUpdateShader* pUpdateShader =
        (CAnimationUpdateShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"AnimationUpdateCS").Get();

    // Bone Data
    CheckBoneFinalMatBuffer();

    pUpdateShader->SetFrameDataBuffer(m_SkeletalMesh->GetBoneFrameDataBuffer());
    pUpdateShader->SetOffsetMatBuffer(m_SkeletalMesh->GetBoneOffsetBuffer());
    pUpdateShader->SetOutputBuffer(m_BoneFinalMatBuffer);

    pUpdateShader->SetBoneCount(m_SkeletalMesh->GetBoneCount());
    pUpdateShader->SetFrameIndex(m_FrameIdx);
    pUpdateShader->SetNextFrameIdx(m_NextFrameIdx);
    pUpdateShader->SetFrameRatio(m_Ratio);

    // 업데이트 쉐이더 실행
    pUpdateShader->Execute();
}

void CAnimator::SetSkeletalMesh(Ptr<CMesh> _SkeletalMesh)
{
    if (nullptr == _SkeletalMesh)
    {
        // Reset
        m_SkeletalMesh = nullptr;
        m_mapClip.clear();
        m_CurClipIdx = 0;
        m_vecClipUpdateTime.clear();
        m_bPlay = true;
        m_bRepeat = true;
        m_PlaySpeed = 1.f;
        m_FrameRate = 30;
        m_CurTime = 0.;
        m_NextFrameIdx = 0;
        m_Ratio = 0.;
        m_bFinalMatUpdate = false;

        if (nullptr != m_BoneFinalMatBuffer)
        {
            delete m_BoneFinalMatBuffer;
            m_BoneFinalMatBuffer = new CStructuredBuffer;
        }

        return;
    }

    m_SkeletalMesh = _SkeletalMesh;

    const vector<tMTAnimClip>* vecAnimClip = m_SkeletalMesh->GetAnimClip();
    for (int i = 0; i < (int)vecAnimClip->size(); ++i)
    {
        const tMTAnimClip& Clip = vecAnimClip->at(i);
        const auto& iter = m_mapClip.insert(make_pair(Clip.strAnimName, i));
        assert(!(iter.first == m_mapClip.end())); // 동일한 Key의 Clip이 존재하였다.
    }

    m_vecClipUpdateTime.resize(vecAnimClip->size());

    // Frame Rate 설정
    if (!vecAnimClip->empty())
    {
        m_FrameRate = FbxTime::GetFrameRate(vecAnimClip->back().eMode);
    }

    CheckBoneFinalMatBuffer();
}

void CAnimator::SetFrameIdx(int _FrameIdx)
{
    double CurFrameTime = (_FrameIdx / m_FrameRate) - m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dStartTime;
    m_vecClipUpdateTime[m_CurClipIdx] = std::clamp(CurFrameTime, 0., m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dTimeLength);
}

UINT CAnimator::GetBoneCount() const
{
    if (nullptr == m_SkeletalMesh)
        return 0;

    return m_SkeletalMesh->GetBoneCount();
}

void CAnimator::ClearData()
{
    m_BoneFinalMatBuffer->Clear(31);

    UINT iMtrlCount = MeshRender()->GetMtrlCount();
    Ptr<CMaterial> pMtrl = nullptr;
    for (UINT i = 0; i < iMtrlCount; ++i)
    {
        pMtrl = MeshRender()->GetSharedMaterial(i);
        if (nullptr == pMtrl)
            continue;

        pMtrl->SetAnim3D(false);
        pMtrl->SetBoneCount(0);
    }
}

int CAnimator::FindClipIndex(const wstring& _strClipName)
{
    map<wstring, int>::iterator iter = m_mapClip.find(_strClipName);
    if (iter == m_mapClip.end())
        return -1; // Key에 해당하는 Clip 을 찾지 못하였다.

    return iter->second;
}

void CAnimator::Play(const wstring& _strClipName, bool _bRepeat, float _PlaySpeed, double _ChangeDuration)
{
    int ClipIndex = FindClipIndex(_strClipName);
    if (-1 == ClipIndex)
        return;

    m_bChanging = true;
    m_CurChangeTime = 0.;
    m_ChangeDuration = _ChangeDuration;
    m_NextClipIdx = ClipIndex;
    m_bNextRepeat = _bRepeat;
    m_NextPlaySpeed = _PlaySpeed;

    m_NextFrameIdx = m_SkeletalMesh->GetAnimClip()->at(m_NextClipIdx).iStartFrame;
}

bool CAnimator::IsFinish() const
{
    // 현재 ClipUpdateTime과 TimeLength의 차이가 작은 경우 Finish
    return 1e-3 > abs(m_vecClipUpdateTime[m_CurClipIdx] - m_SkeletalMesh->GetAnimClip()->at(m_CurClipIdx).dTimeLength);
}

bool CAnimator::IsVaild()
{
    if (nullptr != m_SkeletalMesh && m_SkeletalMesh->IsSkeletalMesh())
    {
        return true;
    }

    return false;
}

void CAnimator::CheckBoneFinalMatBuffer()
{
    if (!IsVaild())
        return;

    UINT iBoneCount = m_SkeletalMesh->GetBoneCount();
    if (m_BoneFinalMatBuffer->GetElementCount() != iBoneCount)
    {
        m_BoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, true, nullptr);
    }
}

void CAnimator::SaveToLevelFile(FILE* _File)
{
    SaveAssetRef(m_SkeletalMesh, _File);

    fwrite(&m_bPlay, 1, sizeof(bool), _File);
    fwrite(&m_bRepeat, 1, sizeof(bool), _File);
    fwrite(&m_PlaySpeed, 1, sizeof(float), _File);
}

void CAnimator::LoadFromLevelFile(FILE* _File)
{
    LoadAssetRef(m_SkeletalMesh, _File);
    SetSkeletalMesh(m_SkeletalMesh);

    fread(&m_bPlay, 1, sizeof(bool), _File);
    fread(&m_bRepeat, 1, sizeof(bool), _File);
    fread(&m_PlaySpeed, 1, sizeof(float), _File);
}
