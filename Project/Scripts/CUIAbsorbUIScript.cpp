#include "pch.h"
#include "CUIAbsorbUIScript.h"

#include <Engine\CRenderMgr.h>

CUIAbsorbUIScript::CUIAbsorbUIScript()
    : CScript(UIABSORBUISCRIPT)
    , m_pObj(nullptr)
    , m_vOffset(Vec3(0.f, 300.f, 0.f))
    , m_eState(AbsorbUIState::Progress)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOffset, "Offset");
}

CUIAbsorbUIScript::CUIAbsorbUIScript(const CUIAbsorbUIScript& Origin)
    : CScript(Origin)
    , m_pObj(nullptr)
    , m_vOffset(Origin.m_vOffset)
    , m_eState(AbsorbUIState::Progress)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOffset, "Offset");
}

CUIAbsorbUIScript::~CUIAbsorbUIScript()
{
}

void CUIAbsorbUIScript::begin()
{
    CMeshRender* pRender = MeshRender();
    if (nullptr != pRender)
    {
        pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
        pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
    }
}

void CUIAbsorbUIScript::tick()
{
    switch (m_eState)
    {
    case AbsorbUIState::Progress: {
        Progress();
    }
    break;
    case AbsorbUIState::End: {
        End();
    }
    break;
    case AbsorbUIState::EndOff: {
        EndOff();
    }
    break;
    default:
        break;
    }
}

void CUIAbsorbUIScript::Progress()
{
    CMeshRender* pRender = MeshRender();
    if (nullptr != pRender)
    {
        if (nullptr != pRender->GetMaterial(0))
        {
            pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
            pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 1.f);
        }

        if (nullptr != m_pObj)
            TargetTrack();
    }
}

void CUIAbsorbUIScript::End()
{
    CMeshRender* pRender = MeshRender();
    if (nullptr != pRender)
    {
        if (nullptr != pRender->GetMaterial(0))
        {
            pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
            pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
        }
    }
}

void CUIAbsorbUIScript::EndOff()
{
    CMeshRender* pRender = MeshRender();
    if (nullptr != pRender)
    {
        if (nullptr != pRender->GetMaterial(0))
        {
            pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
            pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
        }
    }
}

void CUIAbsorbUIScript::TargetTrack()
{
    Vec3 playerPos = m_pObj->Transform()->GetWorldPos();

    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetCamera(0);

    Matrix _VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    // WolrdPos -> NDC
    Vec3 _vPlayerNDCPos = Vector3::Transform(playerPos, _VPMatrix);

    // UICam
    _pCam = CRenderMgr::GetInst()->GetCamera(2);

    if (nullptr == _pCam)
        return;

    // NDC -> WorldPos
    Matrix _VPInverseMatrix = _pCam->GetProjInvMat() * _pCam->GetViewInvMat();

    Vec3 WorldPos = Vector3::Transform(_vPlayerNDCPos, _VPInverseMatrix);

    wstring ObjName = m_pObj->GetName();
    if (ObjName == L"VendingMachine")
    {
        m_vOffset = Vec3(0.f, 386.f, 0.f);
    }
    else
    {
        m_vOffset = Vec3(0.f, 286.f, 0.f);
    }

    GetOwner()->Transform()->SetWorldPos(WorldPos + m_vOffset);
}

UINT CUIAbsorbUIScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CUIAbsorbUIScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}