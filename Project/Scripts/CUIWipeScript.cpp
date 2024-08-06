#include "pch.h"
#include "CUIWipeScript.h"

#include <Engine/CRenderMgr.h>
#include "CPlayerMgr.h"

CUIWipeScript::CUIWipeScript()
    : CScript(UIWIPESCRIPT)
    , m_pPlayer(nullptr)
    , m_vOriginScale{}
    , m_eState(WipeState::Wait)
    , m_bRotate(false)
    , m_fTime(0.f)
    , m_fAccTime(0.f)
    , m_fRotSpeed(0.f)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRotate, "Rotate");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTime, "Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRotSpeed, "Rot Speed");
}

CUIWipeScript::CUIWipeScript(const CUIWipeScript& Origin)
    : CScript(Origin)
    , m_pPlayer(nullptr)
    , m_vOriginScale{}
    , m_eState(WipeState::Wait)
    , m_bRotate(Origin.m_bRotate)
    , m_fTime(0.f)
    , m_fAccTime(0.f)
    , m_fRotSpeed(0.f)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRotate, "Rotate");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTime, "Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRotSpeed, "Rot Speed");
}

CUIWipeScript::~CUIWipeScript()
{
}

void CUIWipeScript::begin()
{
    m_pPlayer = PLAYER;
    m_vOriginScale = GetOwner()->Transform()->GetLocalScale();
    ChangeState(WipeState::Wait);
}

void CUIWipeScript::tick()
{
    switch (m_eState)
    {
    case WipeState::Wait: {
        Wait();
    }
    break;
    case WipeState::Minimize: {
        Minimize();
    }
    break;
    case WipeState::Maximize: {
        Maximize();
    }
    break;
    case WipeState::End:
        break;
    default:
        break;
    }
}

void CUIWipeScript::ChangeState(WipeState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CUIWipeScript::EnterState(WipeState _state)
{
}

void CUIWipeScript::ExitState(WipeState _state)
{
    switch (_state)
    {
    case WipeState::Wait:
        break;
    case WipeState::Minimize: {
        m_vOriginScale = Vec3(1.f, 1.f, 1.f);
        m_fAccTime = 0.f;
    }
    break;
    case WipeState::Maximize: {
        m_fAccTime = 0.f;
    }
    break;
    case WipeState::End:
        break;
    default:
        break;
    }
}

void CUIWipeScript::Wait()
{
}

void CUIWipeScript::Maximize()
{
}

void CUIWipeScript::Minimize()
{
    if (m_fTime == 0.f)
        return;

    m_fAccTime += DT;

    Tracking();

    true == m_bRotate ? Rotate() : void();

    if (m_fTime <= m_fAccTime)
    {
        ChangeState(WipeState::Wait);
        return;
    }

    Vec3 vScale = m_vOriginScale * ((m_fAccTime - m_fTime) / m_fAccTime);
    GetOwner()->Transform()->SetLocalScale(vScale);
}

void CUIWipeScript::Tracking()
{
    Vec3 playerPos = m_pPlayer->Transform()->GetWorldPos();

    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetCamera(0);

    Matrix _VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    // WolrdPos -> NDC
    Vec3 _vPlayerNDCPos = Vector3::Transform(playerPos, _VPMatrix);

    // UICam
    _pCam = CRenderMgr::GetInst()->GetCamera(2);

    // NDC -> WorldPos
    Matrix _VPInverseMatrix = _pCam->GetProjInvMat() * _pCam->GetViewInvMat();

    Vec3 WorldPos = Vector3::Transform(_vPlayerNDCPos, _VPInverseMatrix);

    GetOwner()->Transform()->SetWorldPos(WorldPos);
}

void CUIWipeScript::Rotate()
{
    Vec3 _vRadian = Transform()->GetWorldRotation();

    _vRadian.z += DT * m_fRotSpeed;

    Transform()->SetWorldRotation(_vRadian);
}

UINT CUIWipeScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bRotate, sizeof(bool), 1, _File);
    fwrite(&m_fTime, sizeof(float), 1, _File);
    fwrite(&m_fRotSpeed, sizeof(float), 1, _File);

    MemoryByte = sizeof(bool);
    MemoryByte = sizeof(float);
    MemoryByte = sizeof(float);

    return MemoryByte;
}

UINT CUIWipeScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bRotate, sizeof(bool), 1, _File);
    fread(&m_fTime, sizeof(float), 1, _File);
    fread(&m_fRotSpeed, sizeof(float), 1, _File);

    MemoryByte = sizeof(bool);
    MemoryByte = sizeof(float);
    MemoryByte = sizeof(float);

    return MemoryByte;
}
