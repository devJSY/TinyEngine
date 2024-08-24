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
    , m_vButtonScale{}
    , m_IsInteraction(true)
    , m_IsHovered(true)
    , m_IsIsolated(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_eTransition, "Transition");

    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[0], "Normal Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[1], "Highlighted Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[2], "Pressed Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[3], "Selected Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[4], "Disabled Color");

    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[0], "Normal Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[1], "Highlighted Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[2], "Pressed Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[3], "Selected Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[4], "Disabled Scale");
}

CButtonScript::CButtonScript(UINT _ScriptType)
    : CScript(_ScriptType)
    , m_ePrevState(ButtonState::NORMAL)
    , m_eCurState(ButtonState::NORMAL)
    , m_eTransition(ButtonTransition::TEXTURE)
    , m_pButtonTex{}
    , m_vButtonColor{}
    , m_vButtonScale{}
    , m_IsInteraction(true)
    , m_IsHovered(true)
    , m_IsIsolated(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_eTransition, "Transition");

    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[0], "Normal Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[1], "Highlighted Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[2], "Pressed Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[3], "Selected Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[4], "Disabled Color");

    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[0], "Normal Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[1], "Highlighted Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[2], "Pressed Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[3], "Selected Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[4], "Disabled Scale");
}

CButtonScript::CButtonScript(const CButtonScript& Origin)
    : CScript(Origin)
    , m_ePrevState(ButtonState::NORMAL)
    , m_eCurState(ButtonState::NORMAL)
    , m_eTransition(Origin.m_eTransition)
    , m_pButtonTex{}
    , m_vButtonColor{}
    , m_vButtonScale{}
    , m_IsInteraction(Origin.m_IsInteraction)
    , m_IsHovered(false)
    , m_IsIsolated(Origin.m_IsIsolated)
{
    for (UINT i = 0; i < (UINT)ButtonState::END; i++)
    {
        m_pButtonTex[i] = Origin.m_pButtonTex[i];
        m_vButtonColor[i] = Origin.m_vButtonColor[i];
        m_vButtonScale[i] = Origin.m_vButtonScale[i];
    }

    AddScriptParam(SCRIPT_PARAM::INT, &m_eTransition, "Transition");

    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[0], "Normal Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[1], "Highlighted Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[2], "Pressed Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[3], "Selected Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vButtonColor[4], "Disabled Color");

    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[0], "Normal Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[1], "Highlighted Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[2], "Pressed Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[3], "Selected Scale");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_vButtonScale[4], "Disabled Scale");
}

CButtonScript::~CButtonScript()
{
}

void CButtonScript::Func()
{
    ChangeState(ButtonState::DISABLED);
}

void CButtonScript::begin()
{
    m_vNormalScale = GetOwner()->GetComponent<CTransform>()->GetWorldScale();

    vector<CGameObject*> vChilids = GetOwner()->GetChildObject();
    for (size_t i = 0; i < vChilids.size(); ++i)
    {
        if (vChilids[i]->Transform())
            m_vNormalScales.push_back(vChilids[i]->Transform()->GetLocalScale());
    }

    ChangeState(ButtonState::NORMAL);
}

void CButtonScript::tick()
{
    CTransform* _pTr = GetOwner()->GetComponent<CTransform>();
    assert(_pTr);

    ChangeState(ButtonState::NORMAL);

    if (m_eCurState == ButtonState::DISABLED)
        return;

    if (IsMouseHovered())
    {
        ChangeState(ButtonState::SELECTED);

        if (KEY_PRESSED(LBTN) || KEY_TAP(LBTN))
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

    if (CEditorMgr::GetInst()->IsEnabled())
        _vMousePos = CEditorMgr::GetInst()->GetViewportMousePos();

    Vec2 _vNDCMousePos = Vec2(0.f, 0.f);

    if (CEditorMgr::GetInst()->IsEnabled())
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
    Vec3 _vButtonPos = GetOwner()->GetComponent<CTransform>()->GetLocalPos();
    Vec3 _vButtonScale = GetOwner()->GetComponent<CTransform>()->GetLocalScale();

    bool _isHovered = false;
    // Not Rotate Rect
    // UI Cam
    CCamera* _pCam = CRenderMgr::GetInst()->GetCamera(0);

    // WorldSpcae -> NDCSpace(Button Object)
    Vec3 _vButtonLT = _vButtonPos + Vec2(-_vButtonScale.x, _vButtonScale.y) / 2.f;
    Vec3 _vButtonRB = _vButtonPos + Vec2(_vButtonScale.x, -_vButtonScale.y) / 2.f;

    Matrix _VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    Vec3 _vNDCLT = Vector3::Transform(_vButtonLT, _VPMatrix);
    Vec3 _vNDCRB = Vector3::Transform(_vButtonRB, _VPMatrix);

    if ((_vNDCSMousePos.x >= _vNDCLT.x && _vNDCSMousePos.x <= _vNDCRB.x) && (_vNDCSMousePos.y <= _vNDCLT.y && _vNDCSMousePos.y >= _vNDCRB.y))
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
            _pTr->SetLocalScale(m_vNormalScale * m_vButtonScale[(UINT)m_eCurState]);
    }
    break;
    case ButtonTransition::ANIMATION: {
    }
    break;
    case ButtonTransition::CUSTOM1: {

        vector<CGameObject*> pChildObj = GetOwner()->GetChildObject();

        for (size_t i = 0; i < pChildObj.size(); i++)
        {
            if (nullptr != pChildObj[i])
            {
                pChildObj[i]->SetActive(true);
            }
        }

        CTransform* _pTr = GetOwner()->GetComponent<CTransform>();

        if (_pTr)
        {
            Vec3 vTransScale = m_vNormalScale * m_vButtonScale[(UINT)m_eCurState];
            _pTr->SetWorldScale(Vec3(vTransScale.x, vTransScale.y, m_vNormalScale.z));
        }

        vector<CGameObject*> vChilids = GetOwner()->GetChildObject();
        for (size_t i = 0; i < vChilids.size(); ++i)
        {
            if (vChilids[i]->Transform())
            {
                Vec3 vTransScale = m_vNormalScales[i] * m_vButtonScale[(UINT)m_eCurState];

                vChilids[i]->Transform()->SetLocalScale(Vec3(vTransScale.x, vTransScale.y, m_vNormalScales[i].z));
            }
        }
    }
    break;
    case ButtonTransition::CUSTOM2: {
        if (ButtonState::NORMAL == m_eCurState)
        {
            vector<CGameObject*> pChildObj = GetOwner()->GetChildObject();

            for (size_t i = 0; i < pChildObj.size(); i++)
            {
                if (nullptr != pChildObj[i])
                {
                    if (pChildObj[i]->GetName() != L"UI_FontTexture")
                    {
                        pChildObj[i]->SetActive(false);
                    }
                }
            }
        }
        else if (ButtonState::SELECTED == m_eCurState)
        {
            vector<CGameObject*> pChildObj = GetOwner()->GetChildObject();

            for (size_t i = 0; i < pChildObj.size(); i++)
            {
                if (nullptr != pChildObj[i])
                    pChildObj[i]->SetActive(true);
            }
        }
        else if (ButtonState::PRESSED == m_eCurState)
        {
            vector<CGameObject*> pChildObj = GetOwner()->GetChildObject();
            for (size_t i = 0; i < pChildObj.size(); i++)
            {
                if (nullptr != pChildObj[i])
                    pChildObj[i]->SetActive(true);
            }
        }
        else if (ButtonState::DISABLED == m_eCurState)
        {
            vector<CGameObject*> pChildObj = GetOwner()->GetChildObject();

            for (size_t i = 0; i < pChildObj.size(); i++)
            {
                if (nullptr != pChildObj[i])
                    pChildObj[i]->SetActive(true);
            }
        }
        CTransform* _pTr = GetOwner()->GetComponent<CTransform>();

        if (_pTr)
        {
            Vec3 vTransScale = m_vNormalScale * m_vButtonScale[(UINT)m_eCurState];
            _pTr->SetWorldScale(Vec3(vTransScale.x, vTransScale.y, m_vNormalScale.z));
        }

        vector<CGameObject*> vChilids = GetOwner()->GetChildObject();
        for (size_t i = 0; i < vChilids.size(); ++i)
        {
            if (vChilids[i]->Transform())
            {
                Vec3 vTransScale = m_vNormalScales[i] * m_vButtonScale[(UINT)m_eCurState];

                vChilids[i]->Transform()->SetLocalScale(Vec3(vTransScale.x, vTransScale.y, m_vNormalScales[i].z));
            }
        }
    }
    break;
    case ButtonTransition::END:
        break;
    default:
        break;
    }
}

UINT CButtonScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_eTransition, sizeof(ButtonTransition), 1, _File);
    fwrite(&m_IsInteraction, sizeof(bool), 1, _File);
    fwrite(&m_IsHovered, sizeof(bool), 1, _File);
    fwrite(&m_IsIsolated, sizeof(bool), 1, _File);

    fwrite(&m_vButtonColor, sizeof(Vec4), static_cast<size_t>((UINT)ButtonState::END), _File);
    fwrite(&m_vButtonScale, sizeof(Vec2), static_cast<size_t>((UINT)ButtonState::END), _File);

    MemoryByte += sizeof(ButtonTransition);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec4) * static_cast<size_t>((UINT)ButtonState::END);
    MemoryByte += sizeof(Vec2) * static_cast<size_t>((UINT)ButtonState::END);

    return MemoryByte;
}

UINT CButtonScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_eTransition, sizeof(ButtonTransition), 1, _File);
    fread(&m_IsInteraction, sizeof(bool), 1, _File);
    fread(&m_IsHovered, sizeof(bool), 1, _File);
    fread(&m_IsIsolated, sizeof(bool), 1, _File);

    fread(&m_vButtonColor, sizeof(Vec4), static_cast<size_t>((UINT)ButtonState::END), _File);
    fread(&m_vButtonScale, sizeof(Vec2), static_cast<size_t>((UINT)ButtonState::END), _File);

    MemoryByte += sizeof(ButtonTransition);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec4) * static_cast<size_t>((UINT)ButtonState::END);
    MemoryByte += sizeof(Vec2) * static_cast<size_t>((UINT)ButtonState::END);

    return MemoryByte;
}