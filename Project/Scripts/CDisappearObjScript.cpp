#include "pch.h"
#include "CDisappearObjScript.h"

#include "CPlayerMgr.h"

CDisappearObjScript::CDisappearObjScript()
    : CScript(DISAPPEAROBJSCRIPT)
    , m_pPlayer(nullptr)
    , m_eState(DisappearObjectState::Wait)
    , m_fAccTime(0.f)
    , m_fBreakTime(0.f)
    , m_fCreateTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBreakTime, "Break Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fCreateTime, "Create Time");
}

CDisappearObjScript::CDisappearObjScript(const CDisappearObjScript& Origin)
    : CScript(Origin)
    , m_pPlayer(nullptr)
    , m_eState(DisappearObjectState::Wait)
    , m_fAccTime(0.f)
    , m_fBreakTime(Origin.m_fBreakTime)
    , m_fCreateTime(Origin.m_fCreateTime)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBreakTime, "Break Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fCreateTime, "Create Time");
}

CDisappearObjScript::~CDisappearObjScript()
{
}

void CDisappearObjScript::begin()
{
    m_pPlayer = PLAYER;
    MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, 1.f);
}

void CDisappearObjScript::tick()
{
    switch (m_eState)
    {
    case DisappearObjectState::Wait: {
    }
    break;
    case DisappearObjectState::PreDisappear: {
        PreDisappear();
    }
    break;
    case DisappearObjectState::Disapper: {
        Disappear();
    }
    break;
    case DisappearObjectState::Appear: {
        Appear();
    }
    break;
    case DisappearObjectState::End:
        break;
    default:
        break;
    }
}

void CDisappearObjScript::PreDisappear()
{
    m_fAccTime += DT;
    MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, 1.f - (m_fAccTime / m_fBreakTime));
    if (m_fBreakTime <= m_fAccTime)
    {
        BoxCollider()->SetEnabled(false);
        ChangeState(DisappearObjectState::Disapper);
    }
}

void CDisappearObjScript::Disappear()
{
    m_pPlayer->CharacterController()->SetGrounded(false);
    ChangeState(DisappearObjectState::Appear);
}

void CDisappearObjScript::Appear()
{
    m_fAccTime += DT;
    MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, (m_fAccTime / m_fBreakTime));
    if (m_fCreateTime <= m_fAccTime)
    {
        BoxCollider()->SetEnabled(true);
        ChangeState(DisappearObjectState::Wait);
    }
}

void CDisappearObjScript::ChangeState(DisappearObjectState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CDisappearObjScript::EnterState(DisappearObjectState _state)
{
    switch (_state)
    {
    case DisappearObjectState::Wait:
        break;
    case DisappearObjectState::PreDisappear:
        break;
    case DisappearObjectState::Disapper:
        break;
    case DisappearObjectState::Appear: {
        m_fAccTime = 0.f;
    }
        break;
    case DisappearObjectState::End:
        break;
    default:
        break;
    }
}

void CDisappearObjScript::ExitState(DisappearObjectState _state)
{
    switch (_state)
    {
    case DisappearObjectState::Wait: {
        m_fAccTime = 0.f;
    }
    break;
    case DisappearObjectState::Appear: {
        m_fAccTime = 0.f;
    }
    break;
    case DisappearObjectState::End:
        break;
    default:
        break;
    }
}

void CDisappearObjScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* Obj = _OtherCollider->GetOwner();
    if (L"Body Collider" == Obj->GetName())
    {
        ChangeState(DisappearObjectState::PreDisappear);
    }
}

UINT CDisappearObjScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fBreakTime, sizeof(float), 1, _File);
    fwrite(&m_fCreateTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CDisappearObjScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fBreakTime, sizeof(float), 1, _File);
    fread(&m_fCreateTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}