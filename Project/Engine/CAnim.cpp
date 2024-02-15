#include "pch.h"
#include "CAnim.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "COutputLog.h"

CAnim::CAnim()
    : m_Animator(nullptr)
    , m_CurFrmIdx(0)
    , m_bFinish(false)
    , m_fAccTime(0.f)
    , m_bUseBackGround(false)
{
}

CAnim::~CAnim()
{
}

void CAnim::finaltick()
{
    if (m_bFinish)
        return;

    m_fAccTime += DT;

    if (m_vecFrm[m_CurFrmIdx].Duration < m_fAccTime)
    {
        ++m_CurFrmIdx;
        if (m_vecFrm.size() <= m_CurFrmIdx)
        {
            m_CurFrmIdx = (int)m_vecFrm.size() - 1;
            m_bFinish = true;
        }
        m_fAccTime = 0.f;
    }
}

void CAnim::UpdateData()
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

    // 아틀라스 텍스쳐 t10 에 바인딩
    m_AtlasTex->UpdateData(10);
}

void CAnim::Clear()
{
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
    tAnimData2D data = {};
    data.UseAnim2D = 0;
    data.UseBackGround = 0;

    pCB->SetData(&data);
    pCB->UpdateData();
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset,
                   Vec2 _vBackground, int _FrmCount, float _FPS, bool _UseBackGround)
{
    m_Animator = _Animator;
    m_AtlasTex = _Atlas;
    m_bUseBackGround = _UseBackGround;

    Vec2 AtlasSize = Vec2((float)_Atlas->GetWidth(), (float)_Atlas->GetHeight());

    for (int i = 0; i < _FrmCount; ++i)
    {
        tAnimFrm frm = {};

        frm.vSlice = Vec2(_vSliceSize.x, _vSliceSize.y) / AtlasSize;

        frm.vLeftTop = Vec2(_vLeftTop.x, _vLeftTop.y) / AtlasSize;
        frm.vLeftTop.x += frm.vSlice.x * i;

        frm.vOffset = Vec2(_vOffset.x, _vOffset.y) / AtlasSize;
        frm.vBackground = Vec2(_vBackground.x, _vBackground.y) / AtlasSize;
        frm.Duration = 1.f / _FPS;

        m_vecFrm.push_back(frm);
    }
}

void CAnim::SaveToLevelFile(FILE* _File)
{
    SaveWString(GetName(), _File);

    size_t FrameCount = m_vecFrm.size();
    fwrite(&FrameCount, sizeof(size_t), 1, _File);
    fwrite(m_vecFrm.data(), sizeof(tAnimFrm), FrameCount, _File);
    fwrite(&m_bUseBackGround, sizeof(bool), 1, _File);
    SaveAssetRef<CTexture>(m_AtlasTex.Get(), _File);
}

void CAnim::LoadFromLevelFile(FILE* _File)
{
    wstring name;
    LoadWString(name, _File);
    SetName(name);

    size_t FrameCount = 0;
    fread(&FrameCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < FrameCount; ++i)
    {
        tAnimFrm frm = {};
        fread(&frm, sizeof(tAnimFrm), 1, _File);
        m_vecFrm.push_back(frm);
    }
    fread(&m_bUseBackGround, sizeof(bool), 1, _File);
    LoadAssetRef<CTexture>(m_AtlasTex, _File);
}

bool CAnim::SaveAnim(const wstring& _FilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _FilePath.c_str(), L"w");

    if (nullptr == pFile)
    {
        LOG(Warning, "파일 열기 실패");
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

bool CAnim::LoadAnim(const wstring& _FilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _FilePath.c_str(), L"r");

    if (nullptr == pFile)
    {
        LOG(Warning, "파일 열기 실패");
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

            fwscanf_s(pFile, L"%s", szRead, 256);
            strKey = szRead;

            fwscanf_s(pFile, L"%s", szRead, 256);
            strRelativePath = szRead;

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
