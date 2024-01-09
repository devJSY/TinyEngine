#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"
#include "CPathMgr.h"

CAnimator2D::CAnimator2D()
    : CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_CurAnim(nullptr)
    , m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
    Delete_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
    if (nullptr != m_CurAnim)
    {
        if (m_bRepeat && m_CurAnim->IsFinish())
        {
            m_CurAnim->Reset();
        }

        m_CurAnim->finaltick();
    }
}

void CAnimator2D::UpdateData()
{
    m_CurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
    CAnim::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize,
                         Vec2 _OffsetPos, Vec2 _Background, int _FrmCount, float _FPS, bool _UseBackGround)
{
    CAnim* pAnim = FindAnim(_strKey);
    assert(!pAnim);

    pAnim = new CAnim;
    pAnim->SetName(_strKey);
    pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetPos, _Background, _FrmCount, _FPS, _UseBackGround);
    m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strKey)
{
    map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strKey);

    if (iter == m_mapAnim.end())
        return nullptr;

    return iter->second;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
    CAnim* pAnim = FindAnim(_strAnimName);
    if (nullptr == pAnim)
        return;

    m_CurAnim = pAnim;
    m_bRepeat = _bRepeat;
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_bRepeat, sizeof(bool), 1, _File);

    size_t AnimCount = m_mapAnim.size();
    fwrite(&AnimCount, sizeof(size_t), 1, _File);

    for (const auto& pair : m_mapAnim)
    {
        pair.second->SaveToLevelFile(_File);
    }

    wstring strCurAnimName;
    if (nullptr != m_CurAnim)
    {
        strCurAnimName = m_CurAnim->GetName();
    }
    SaveWString(strCurAnimName, _File);
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_bRepeat, sizeof(bool), 1, _File);

    size_t AnimCount = 0;
    fread(&AnimCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < AnimCount; ++i)
    {
        CAnim* pNewAnim = new CAnim;
        pNewAnim->LoadFromLevelFile(_File);

        m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
        pNewAnim->m_Animator = this;
    }

    wstring strCurAnimName;
    LoadWString(strCurAnimName, _File);

    m_CurAnim = FindAnim(strCurAnimName);
}

void CAnimator2D::SaveAnimations(const wstring& _strRelativePath)
{
    wstring strFolderPath = CPathMgr::GetContentPath();
    strFolderPath += _strRelativePath;

    for (const auto& pair : m_mapAnim)
    {
        wstring strFilePath = strFolderPath + L"\\" + pair.first + L".anim";
        if (!pair.second->SaveAnim(strFilePath))
        {
            std::cout << "Animation Save 실패" << std::endl;
        }
    }
}

void CAnimator2D::LoadAnimation(const wstring& _strRelativePath)
{
    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    CAnim* pNewAnim = new CAnim;

    if (!pNewAnim->LoadAnim(strFilePath))
    {
        std::cout << "Animation Load 실패" << std::endl;
        delete pNewAnim;
        return;
    }

    pNewAnim->m_Animator = this;
    m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
}
