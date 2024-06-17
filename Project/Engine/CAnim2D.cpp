#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CAnim2D::CAnim2D()
    : m_Animator(nullptr)
    , m_vecFrm{}
    , m_CurFrmIdx(0)
    , m_bFinish(false)
    , m_AtlasTex(nullptr)
    , m_fAccTime(0.f)
    , m_bUseBackGround(false)
{
}

CAnim2D::CAnim2D(const CAnim2D& origin)
    : CEntity(origin)
    , m_Animator(nullptr)
    , m_vecFrm(origin.m_vecFrm)
    , m_CurFrmIdx(origin.m_CurFrmIdx)
    , m_bFinish(origin.m_bFinish)
    , m_AtlasTex(origin.m_AtlasTex)
    , m_fAccTime(origin.m_fAccTime)
    , m_bUseBackGround(origin.m_bUseBackGround)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{
    if (m_bFinish)
        return;

    m_fAccTime += DT;

    if (m_vecFrm[m_CurFrmIdx].Duration < m_fAccTime)
    {
        m_fAccTime -= m_vecFrm[m_CurFrmIdx].Duration;
        ++m_CurFrmIdx;
        if (m_vecFrm.size() <= m_CurFrmIdx)
        {
            m_CurFrmIdx = (int)m_vecFrm.size() - 1;
            m_bFinish = true;
        }
    }
}

void CAnim2D::UpdateData()
{
    // 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
    tAnimData2D data = {};
    data.UseAnim2D = 1;
    data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
    data.vSliceSize = m_vecFrm[m_CurFrmIdx].vSlice;
    data.vBackGround = m_vecFrm[m_CurFrmIdx].vBackground;
    data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
    data.UseBackGround = m_bUseBackGround;

    pCB->SetData(&data);
    pCB->UpdateData();

    // 아틀라스 텍스쳐 t12 에 바인딩
    m_AtlasTex->UpdateData(12);
}

void CAnim2D::Clear()
{
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
    tAnimData2D data = {};
    data.UseAnim2D = 0;
    data.UseBackGround = 0;

    pCB->SetData(&data);
    pCB->UpdateData();
}

void CAnim2D::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount,
                     float _FPS, bool _UseBackGround)
{
    m_Animator = _Animator;
    m_AtlasTex = _Atlas;
    m_bUseBackGround = _UseBackGround;

    Vec2 AtlasSize = Vec2((float)_Atlas->GetWidth(), (float)_Atlas->GetHeight());

    for (int i = 0; i < _FrmCount; ++i)
    {
        tAnim2DFrm frm = {};

        frm.vSlice = Vec2(_vSliceSize.x, _vSliceSize.y) / AtlasSize;

        frm.vLeftTop = Vec2(_vLeftTop.x, _vLeftTop.y) / AtlasSize;
        frm.vLeftTop.x += frm.vSlice.x * i;

        frm.vOffset = Vec2(_vOffset.x, _vOffset.y) / AtlasSize;
        frm.vBackground = Vec2(_vBackground.x, _vBackground.y) / AtlasSize;
        frm.Duration = 1.f / _FPS;

        m_vecFrm.push_back(frm);
    }
}

bool CAnim2D::SaveAnim(const wstring& _FilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _FilePath.c_str(), L"w");

    if (nullptr == pFile)
    {
        LOG(Warning, "File Open Failed!");
        return false;
    }

    // Animation 이름 저장
    fwprintf_s(pFile, L"[ANIM_NAME]\n");

    wstring strName = GetName();
    fwprintf_s(pFile, strName.c_str());
    fwprintf_s(pFile, L"\n\n");

    // Atlas Texture
    fwprintf_s(pFile, L"[ATLAS_TEXTURE]\n");

    wstring strKey;
    wstring strRelativePath;

    if (nullptr != m_AtlasTex.Get())
    {
        strKey = m_AtlasTex->GetKey();
        strRelativePath = m_AtlasTex->GetRelativePath();
    }

    fwprintf_s(pFile, strKey.c_str());
    fwprintf_s(pFile, L"\n");
    fwprintf_s(pFile, strRelativePath.c_str());
    fwprintf_s(pFile, L"\n\n");

    // 프레임 데이터
    // 프레임 숫자
    fwprintf_s(pFile, L"[FRAME_COUNT]\n");
    fwprintf_s(pFile, L"%d\n\n", (int)m_vecFrm.size());

    for (int i = 0; i < (int)m_vecFrm.size(); ++i)
    {
        fwprintf_s(pFile, L"[FRAME_INDEX]\n");
        fwprintf_s(pFile, L"%d\n", i);

        fwprintf_s(pFile, L"[LEFT_TOP]\n");
        fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vLeftTop.x, m_vecFrm[i].vLeftTop.y);

        fwprintf_s(pFile, L"[SLICE_SIZE]\n");
        fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vSlice.x, m_vecFrm[i].vSlice.y);

        fwprintf_s(pFile, L"[OFFSET]\n");
        fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vOffset.x, m_vecFrm[i].vOffset.y);

        fwprintf_s(pFile, L"[BACKGROUND]\n");
        fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].vBackground.x, m_vecFrm[i].vBackground.y);

        fwprintf_s(pFile, L"[DURATION]\n");
        fwprintf_s(pFile, L"%f\n\n", m_vecFrm[i].Duration);
    }

    // Use BackGround
    fwprintf_s(pFile, L"[USE_BACKGROUND]\n");
    fwprintf_s(pFile, L"%s", m_bUseBackGround ? L"true" : L"false");

    fclose(pFile);

    return true;
}

bool CAnim2D::LoadAnim(const wstring& _FilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _FilePath.c_str(), L"r");
    std::filesystem::path filePath = _FilePath;

    if (nullptr == pFile)
    {
        LOG(Warning, "File Open Failed!");
        return false;
    }

    if (L".anim" != filePath.extension())
    {
        LOG(Warning, "File Open Failed!");
        fclose(pFile);
        return false;
    }

    // Animation 이름 로드
    while (true)
    {
        wchar_t szRead[256] = {};
        if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
        {
            break;
        }

        if (!wcscmp(szRead, L"[ANIM_NAME]"))
        {
            fwscanf_s(pFile, L"%s", szRead, 256);
            SetName(szRead);
        }
        else if (!wcscmp(szRead, L"[ATLAS_TEXTURE]"))
        {
            wstring strKey, strRelativePath;

            // 공백포함 문자열 읽기
            fgetws(szRead, sizeof(szRead), pFile); // '\n' 제거
            fgetws(szRead, sizeof(szRead), pFile);
            strKey = szRead;
            strKey.erase(remove(strKey.begin(), strKey.end(), '\n'), strKey.end()); // '\n' 제거

            fgetws(szRead, sizeof(szRead), pFile);
            strRelativePath = szRead;
            strRelativePath.erase(remove(strRelativePath.begin(), strRelativePath.end(), '\n'), strRelativePath.end()); // '\n' 제거

            m_AtlasTex = CAssetMgr::GetInst()->Load<CTexture>(strKey, strRelativePath);
        }
        else if (!wcscmp(szRead, L"[FRAME_COUNT]"))
        {
            int iFrameCount = 0;
            fwscanf_s(pFile, L"%d", &iFrameCount);
            m_vecFrm.resize(iFrameCount);

            int iCurFrame = 0;
            while (true)
            {
                fwscanf_s(pFile, L"%s", szRead, 256);

                if (!wcscmp(szRead, L"[FRAME_INDEX]"))
                {
                    fwscanf_s(pFile, L"%d", &iCurFrame);
                }
                else if (!wcscmp(szRead, L"[LEFT_TOP]"))
                {
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vLeftTop.x);
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vLeftTop.y);
                }
                else if (!wcscmp(szRead, L"[SLICE_SIZE]"))
                {
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vSlice.x);
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vSlice.y);
                }
                else if (!wcscmp(szRead, L"[OFFSET]"))
                {
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vOffset.x);
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vOffset.y);
                }
                else if (!wcscmp(szRead, L"[BACKGROUND]"))
                {
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vBackground.x);
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].vBackground.y);
                }
                else if (!wcscmp(szRead, L"[DURATION]"))
                {
                    fwscanf_s(pFile, L"%f", &m_vecFrm[iCurFrame].Duration);
                }
                else if (!wcscmp(szRead, L"[USE_BACKGROUND]"))
                {
                    fwscanf_s(pFile, L"%s", szRead, 256);

                    if (!wcscmp(szRead, L"true"))
                        m_bUseBackGround = true;
                    else
                        m_bUseBackGround = false;

                    if (iFrameCount - 1 <= iCurFrame)
                        break;
                }
            }
        }
    }

    fclose(pFile);

    return true;
}

void CAnim2D::SaveToLevelFile(FILE* _File)
{
    // 애니메이션 이름 저장
    SaveWStringToFile(GetName(), _File);

    // 모든 프레임 정보 저장
    size_t FrameCount = m_vecFrm.size();
    fwrite(&FrameCount, sizeof(size_t), 1, _File);
    fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, _File);

    // 백그라운드 사용여부 저장
    fwrite(&m_bUseBackGround, sizeof(bool), 1, _File);

    // 참조하던 텍스쳐 정보 저장
    SaveAssetRef(m_AtlasTex, _File);
}

void CAnim2D::LoadFromLevelFile(FILE* _File)
{
    // 애니메이션 이름 로드
    wstring name;
    LoadWStringFromFile(name, _File);
    SetName(name);

    // 모든 프레임 정보 로드
    size_t FrameCount = 0;
    fread(&FrameCount, sizeof(size_t), 1, _File);
    m_vecFrm.resize(FrameCount);
    fread(m_vecFrm.data(), sizeof(tAnim2DFrm), m_vecFrm.size(), _File);

    // 백그라운드 사용여부 로드
    fread(&m_bUseBackGround, sizeof(bool), 1, _File);

    // 참조하던 텍스쳐 정보 로드
    LoadAssetRef(m_AtlasTex, _File);
}
