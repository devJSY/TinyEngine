#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

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
    m_CurAnim->finaltick();
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

void CAnimator2D::Play(const wstring& _strAnimName)
{
    CAnim* pAnim = FindAnim(_strAnimName);
    if (nullptr == pAnim)
        return;

    m_CurAnim = pAnim;
}
