#include "pch.h"
#include "CButtonScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CEngine.h>
#include <Engine/CEditorMgr.h>
#include <Engine/CRenderMgr.h>

CButtonScript::CButtonScript()
    : CScript(BUTTONSCRIPT)
    , m_ePrevState(ButtonState::NORMAL)
    , m_eCurState(ButtonState::NORMAL)
    , m_eTransition(ButtonTransition::TEXTURE)
    , m_pButtonTex{}
    , m_vButtonColor{}
    , m_IsInteraction(true)
    , m_IsHovered(true)
    , m_IsIsolated(false)
{
}

CButtonScript::~CButtonScript()
{
}

void CButtonScript::begin()
{
    m_vNormalScale = GetOwner()->GetComponent<CTransform>()->GetRelativeScale();
}

void CButtonScript::tick()
{
    CTransform* _pTr = GetOwner()->GetComponent<CTransform>();
    assert(_pTr);

    if (IsMouseHovered() && m_IsIsolated)
    {
        ChangeState(ButtonState::SELECTED);

        if (KEY_TAP(LBTN))
        {
            ChangeState(ButtonState::PRESSED);
        }
    }

    ButtonUpdate();
}

bool CButtonScript::IsMouseHovered()
{
    // ScreenSpace MousePos
    Vec2 _vMousePos = CKeyMgr::GetInst()->GetMousePos();

    if (CEditorMgr::GetInst()->IsEnable())
        _vMousePos = CEditorMgr::GetInst()->GetViewportMousePos();

    Vec2 _vNDCMousePos = Vec2(0.f, 0.f);

    if (CEditorMgr::GetInst()->IsEnable())
    {
        Vec2 _vEditorResol = CEditorMgr::GetInst()->GetViewportSize();

        _vNDCMousePos.x = ((_vMousePos.x * 2.f) - (_vEditorResol.x - 1.f)) / (_vEditorResol.x - 1.f);
        _vNDCMousePos.y = ((_vMousePos.y * -2.f) + (_vEditorResol.y - 1.f)) / (_vEditorResol.y - 1.f);
    }
    else
    {
        Vec2 _vResolution = CEngine::GetInst()->GetResolution();

        _vNDCMousePos.x = ((_vMousePos.x * 2.f) - (_vResolution.x - 1.f)) / (_vResolution.x - 1.f);
        _vNDCMousePos.y = ((_vMousePos.y * -2.f) + (_vResolution.y - 1.f)) / (_vResolution.y - 1.f);
    }

    m_IsHovered = IsRectInPoint(_vNDCMousePos);

    return m_IsHovered;
}

bool CButtonScript::IsRectInPoint(Vec2 _vNDCSMousePos)
{
    Vec3 _vButtonPos = GetOwner()->GetComponent<CTransform>()->GetRelativePos();
    Vec3 _vButtonScale = GetOwner()->GetComponent<CTransform>()->GetRelativeScale();

    bool _isHovered = false;
    // Not Rotate Rect
    // UI Cam
    CCamera* _pCam = CRenderMgr::GetInst()->GetCamera(1);

    // WorldSpcae -> NDCSpace(Button Object)
    Vec3 _vButtonLT = _vButtonPos + Vec2(-_vButtonScale.x, _vButtonScale.y) / 2.f;
    Vec3 _vButtonRB = _vButtonPos + Vec2(_vButtonScale.x, -_vButtonScale.y) / 2.f;

    Matrix _VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    Vec3 _vNDCLT = Vector3::Transform(_vButtonLT, _VPMatrix);
    Vec3 _vNDCRB = Vector3::Transform(_vButtonRB, _VPMatrix);

    if ((_vNDCSMousePos.x >= _vNDCLT.x && _vNDCSMousePos.x <= _vNDCRB.x) 
        && (_vNDCSMousePos.y <= _vNDCLT.y && _vNDCSMousePos.y >= _vNDCRB.y))
    {
        _isHovered = true;
    }

    // RotateZ Rect

    return _isHovered;
}

void CButtonScript::ButtonUpdate()
{
    switch (m_eTransition)
    {
    case ButtonTransition::COLOR: {
        CMeshRender* _pRender = GetOwner()->GetComponent<CMeshRender>();
        if (_pRender)
            _pRender->GetMaterial(0)->SetScalarParam(VEC4_0, m_vButtonColor[(UINT)m_eCurState]);
    }
    break;
    case ButtonTransition::TEXTURE: {
        CMeshRender* _pRender = GetOwner()->GetComponent<CMeshRender>();
        if (_pRender)
            _pRender->GetMaterial(0)->SetTexParam(TEX_0, m_pButtonTex[(UINT)m_eCurState]);
    }
    break;
    case ButtonTransition::SCALE: {
        CTransform* _pTr = GetOwner()->GetComponent<CTransform>();
        if (_pTr)
            _pTr->SetRelativeScale(m_vNormalScale * m_vButtonScale[(UINT)m_eCurState]);
    }
    break;
    case ButtonTransition::ANIMATION: {
    }
    break;
    case ButtonTransition::END:
        break;
    default:
        break;
    }
}