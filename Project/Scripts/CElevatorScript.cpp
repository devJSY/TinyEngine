#include "pch.h"
#include "CElevatorScript.h"

CElevatorScript::CElevatorScript()
    : CScript(ELEVATORSCRIPT)
    , m_pPlayer(nullptr)
    , m_vDest{}
    , m_eState(ElevatorState::Stop)
    , m_fSpeed(0.f)
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
}

CElevatorScript::CElevatorScript(const CElevatorScript& Origin)
    : CScript(Origin)
    , m_pPlayer(nullptr)
    , m_vDest{}
    , m_eState(ElevatorState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
}

CElevatorScript::~CElevatorScript()
{
}

void CElevatorScript::tick()
{
    switch (m_eState)
    {
    case ElevatorState::Move: {
        Move();
    }
    break;
    case ElevatorState::Stop: {
        Stop();
    }
    break;
    case ElevatorState::End:
        break;
    default:
        break;
    }
}

void CElevatorScript::Move()
{
    Vec3 vPos = Transform()->GetLocalPos();
    if ((m_vDest.x - 1.f <= vPos.x && vPos.x <= m_vDest.x + 1.f) && (m_vDest.y - 1.f <= vPos.y && vPos.y <= m_vDest.y + 1.f) &&
        (m_vDest.z - 1.f <= vPos.z && vPos.z <= m_vDest.z + 1.f))
    {
        m_eState = ElevatorState::Stop;
    }
    else
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 1.f, 0.f) * m_fSpeed * DT);
    }

    if (m_bFlag)
    {
        m_bFlag = false;
        nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(Vec3(0.f, 0.001f, 0.f)) : void();
    }
    else
    {
        m_bFlag = true;
        nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(Vec3(0.f, -0.001f, 0.f)) : void();
    }
}

void CElevatorScript::Stop()
{
    Transform()->SetLocalPos(m_vDest);
    Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
}

void CElevatorScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
        m_pPlayer = pObj->GetParent();
}

void CElevatorScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
        m_pPlayer = nullptr;
}

UINT CElevatorScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_vDest, sizeof(Vec3), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CElevatorScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_vDest, sizeof(Vec3), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}