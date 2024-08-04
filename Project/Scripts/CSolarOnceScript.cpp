#include "pch.h"
#include "CSolarOnceScript.h"

#include "CMovingObjScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CSolarOnceScript::CSolarOnceScript()
    : CScript(SOLARONCESCRIPT)
    , m_MovingObjName{}
    , m_eState(SolarOnceState::OffWait)
    , m_IsArea(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarOnceScript::CSolarOnceScript(const CSolarOnceScript& Origin)
    : CScript(Origin)
    , m_MovingObjName{}
    , m_eState(SolarOnceState::OffWait)
    , m_IsArea(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarOnceScript::~CSolarOnceScript()
{
}

void CSolarOnceScript::begin()
{
    ChangeState(SolarOnceState::OffWait);
}

void CSolarOnceScript::tick()
{
    switch (m_eState)
    {
    case SolarOnceState::ChargeOn: {
        ChargeOn();
    }
    break;
    case SolarOnceState::ChargeOff: {
        ChargeOff();
    }
    break;
    case SolarOnceState::OnWait:
        break;
    case SolarOnceState::OnWaitStart: {

        OnWaitStart();
    }
    break;
    case SolarOnceState::OffWait: {
        OffWait();
    }
    break;
    case SolarOnceState::End:
        break;
    default:
        break;
    }
}

void CSolarOnceScript::ChangeState(SolarOnceState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CSolarOnceScript::EnterState(SolarOnceState _state)
{
    switch (_state)
    {
    case SolarOnceState::ChargeOn: {
        Animator()->Play(ANIMPREFIX("Charge"), false);
    }
    break;
    case SolarOnceState::ChargeOff: {
        Animator()->SetReverse(true);
    }
    break;
    case SolarOnceState::OnWait: {
        Animator()->Play(ANIMPREFIX("OnWait"), false);
    }
    break;
    case SolarOnceState::OnWaitStart: {
        CLevelMgr::GetInst()
            ->GetCurrentLevel()
            ->FindObjectByName(ToWstring(m_MovingObjName), 2)
            ->GetScript<CMovingObjScript>()
            ->SetState(MovingObjState::Move);
        Animator()->Play(ANIMPREFIX("OnWaitStart"), false);
    }
    break;
    case SolarOnceState::OffWait: {
        Animator()->Play(ANIMPREFIX("OffWait"), false);
    }
    break;
    case SolarOnceState::End:
        break;
    default:
        break;
    }
}

void CSolarOnceScript::ExitState(SolarOnceState _state)
{
}

void CSolarOnceScript::ChargeOn()
{
    if (!m_IsArea || !PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarOnceState::ChargeOff);
    }

    if (Animator()->IsFinish())
    {
        ChangeState(SolarOnceState::OnWaitStart);
    }
}

void CSolarOnceScript::ChargeOff()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarOnceState::OffWait);
    }
}

void CSolarOnceScript::OnWaitStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarOnceState::OnWait);
    }
}

void CSolarOnceScript::OffWait()
{
    if (m_IsArea && PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarOnceState::ChargeOn);
    }
}

void CSolarOnceScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = true;
    }
}

void CSolarOnceScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = false;
    }
}

UINT CSolarOnceScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = ToWstring(m_MovingObjName);
    SaveWStringToFile(wStr, _File);

    MemoryByte += sizeof(wStr);

    return MemoryByte;
}

UINT CSolarOnceScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = {};
    LoadWStringFromFile(wStr, _File);

    m_MovingObjName = ToString(wStr);

    MemoryByte += sizeof(wStr);

    return MemoryByte;
}