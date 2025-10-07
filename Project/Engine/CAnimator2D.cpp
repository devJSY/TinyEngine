#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"
#include "CPathMgr.h"

CAnimator2D::CAnimator2D()
    : CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_UpdateMode(AnimatorUpdateMode::Normal)
    , m_mapAnim{}
    , m_CurAnim(nullptr)
    , m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& origin)
    : CComponent(origin)
    , m_UpdateMode(origin.m_UpdateMode)
    , m_mapAnim{}
    , m_CurAnim(nullptr)
    , m_bRepeat(origin.m_bRepeat)
{
    map<wstring, CAnim2D*>::const_iterator iter = origin.m_mapAnim.begin();
    for (; iter != origin.m_mapAnim.end(); ++iter)
    {
        CAnim2D* pCloneAnim = iter->second->Clone();

        pCloneAnim->m_Animator = this;
        m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
    }

    if (nullptr != origin.m_CurAnim)
    {
        m_CurAnim = FindAnim(origin.m_CurAnim->GetName());
    }
}

CAnimator2D::~CAnimator2D()
{
    Delete_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
    if (nullptr == m_CurAnim)
        return;

    if (m_CurAnim->IsFinish() && m_bRepeat)
    {
        m_CurAnim->Reset();
    }

    m_CurAnim->finaltick();
}

void CAnimator2D::UpdateData()
{
    if (nullptr == m_CurAnim)
        return;

    m_CurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
    CAnim2D::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize, Vec2 _OffsetPos, Vec2 _Background,
                         int _FrmCount, float _FPS, bool _UseBackGround)
{
    CAnim2D* pAnim = FindAnim(_strKey);
    if (nullptr != pAnim)
    {
        assert(nullptr);
        return;
    }

    pAnim = new CAnim2D;
    pAnim->SetName(_strKey);
    pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetPos, _Background, _FrmCount, _FPS, _UseBackGround);
    m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strKey)
{
    map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strKey);

    if (iter == m_mapAnim.end())
        return nullptr;

    return iter->second;
}

bool CAnimator2D::DeleteAnim(const wstring& _strAnimName)
{
    CAnim2D* deleteAnim = FindAnim(_strAnimName);
    if (nullptr == deleteAnim)
        return false;

    m_mapAnim.erase(_strAnimName);

    if (deleteAnim == m_CurAnim)
        m_CurAnim = nullptr;

    delete deleteAnim;
    deleteAnim = nullptr;

    return true;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
    CAnim2D* pAnim = FindAnim(_strAnimName);
    if (nullptr == pAnim)
        return;

    m_bRepeat = _bRepeat;
    m_CurAnim = pAnim;
    m_CurAnim->Reset();
}

bool CAnimator2D::IsFinish() const
{
    if (nullptr == m_CurAnim)
        return false;

    return m_CurAnim->IsFinish();
}

UINT CAnimator2D::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_UpdateMode, sizeof(AnimatorUpdateMode), 1, _File);

    size_t AnimCount = m_mapAnim.size();
    fwrite(&AnimCount, sizeof(size_t), 1, _File);

    for (const auto& pair : m_mapAnim)
    {
        MemoryByte += pair.second->SaveToLevelFile(_File);
    }

    wstring strCurAnimName;
    if (nullptr != m_CurAnim)
    {
        strCurAnimName = m_CurAnim->GetName();
    }
    MemoryByte += SaveWStringToFile(strCurAnimName, _File);

    fwrite(&m_bRepeat, sizeof(bool), 1, _File);

    MemoryByte += sizeof(AnimatorUpdateMode);
    MemoryByte += sizeof(size_t);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CAnimator2D::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_UpdateMode, sizeof(AnimatorUpdateMode), 1, _File);

    size_t AnimCount = 0;
    fread(&AnimCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < AnimCount; ++i)
    {
        CAnim2D* pNewAnim = new CAnim2D;
        MemoryByte += pNewAnim->LoadFromLevelFile(_File);

        m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
        pNewAnim->m_Animator = this;
    }

    wstring strCurAnimName;
    MemoryByte += LoadWStringFromFile(strCurAnimName, _File);

    m_CurAnim = FindAnim(strCurAnimName);

    fread(&m_bRepeat, sizeof(bool), 1, _File);

    MemoryByte += sizeof(AnimatorUpdateMode);
    MemoryByte += sizeof(size_t);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

void CAnimator2D::SaveAnimations(const wstring& _strRelativePath)
{
    wstring strFolderPath = CPathMgr::GetContentPath();
    strFolderPath += _strRelativePath;

    for (const auto& pair : m_mapAnim)
    {
        wstring strFilePath = strFolderPath + L"/" + pair.first + L".anim";
        if (!pair.second->SaveAnim(strFilePath))
        {
            LOG(Warning, "Animation Save Failed!");
        }
    }
}

void CAnimator2D::LoadAnimation(const wstring& _strRelativePath)
{
    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    CAnim2D* pNewAnim = new CAnim2D;

    if (!pNewAnim->LoadAnim(strFilePath))
    {
        LOG(Warning, "Animation Load Failed!");
        delete pNewAnim;
        return;
    }

    // �̹� �ε�� �ִϸ��̼��� �ִ��� üũ
    if (FindAnim(pNewAnim->GetName()))
    {
        LOG(Warning, "Animation Already Exists Animation Reloaded!");
        DeleteAnim(pNewAnim->GetName()); // ���� �ִϸ��̼� ����
    }

    pNewAnim->m_Animator = this;
    m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
}
