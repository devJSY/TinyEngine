#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

CAnimator2D::CAnimator2D()
    : CComponent(COMPONENT_TYPE::ANIMATOR2D)
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
                         Vec2 _OffsetSize, int _FrmCount, float _FPS)
{
    CAnim* pAnim = FindAnim(_strKey);
    assert(!pAnim);

    pAnim = new CAnim;
    pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetSize, _FrmCount, _FPS);
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
