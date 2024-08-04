#include "pch.h"
#include "CSolarChargeScript.h"

#include "CMovingObjScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CSolarChargeScript::CSolarChargeScript()
    : CScript(SOLARCHARGESCRIPT)
    , m_MovingObjName{}
    , m_eState(SolarChargeState::OffWaitStart)
    , m_IsArea(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarChargeScript::CSolarChargeScript(const CSolarChargeScript& Origin)
    : CScript(Origin)
    , m_MovingObjName{}
    , m_eState(SolarChargeState::OffWaitStart)
    , m_IsArea(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarChargeScript::~CSolarChargeScript()
{
}

void CSolarChargeScript::begin()
{
    ChangeState(SolarChargeState::OffWaitStart);
}

void CSolarChargeScript::tick()
{
    switch (m_eState)
    {
    case SolarChargeState::OffWait: {
        OffWait();
    }
    break;
    case SolarChargeState::ChargeOn:
    {
        ChargeOn();
    }
        break;
    case SolarChargeState::ChargeOff:
    {
        ChargeOff();
    }
        break;
    case SolarChargeState::ChargedStart:
    {
        ChargedStart();
    }
        break;
    case SolarChargeState::ChargedWait:
        break;
    case SolarChargeState::Decreases:
    {
        Decrease();
    }
        break;
    case SolarChargeState::OffWaitStart: {
        OffWaitStart();
    }
    break;
    case SolarChargeState::End:
        break;
    default:
        break;
    }
}

void CSolarChargeScript::ChangeState(SolarChargeState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CSolarChargeScript::EnterState()
{
    switch (m_eState)
    {
    case SolarChargeState::OffWait: {
        Animator()->Play(ANIMPREFIX("OffWait"), false);
    }
    break;
    case SolarChargeState::ChargeOn: {
        Animator()->Play(ANIMPREFIX("Charge"), false);
    }
    break;
    case SolarChargeState::ChargeOff: {
        Animator()->SetReverse(true);
    }
    break;
    case SolarChargeState::ChargedStart: {
        Animator()->Play(ANIMPREFIX("ChargedStart"), false);
    }
    break;
    case SolarChargeState::ChargedWait:
        break;
    case SolarChargeState::Decreases: {
        CLevelMgr::GetInst()
            ->GetCurrentLevel()
            ->FindObjectByName(ToWstring(m_MovingObjName), 2)
            ->GetScript<CMovingObjScript>()
            ->SetState(MovingObjState::Move);
        Animator()->Play(ANIMPREFIX("Decreases"), false);
    }
    break;
    case SolarChargeState::OffWaitStart: {
        Animator()->Play(ANIMPREFIX("OffWaitStart"), false);
    }
    break;
    case SolarChargeState::End:
        break;
    default:
        break;
    }
}

void CSolarChargeScript::ExitState()
{
}

void CSolarChargeScript::OffWait()
{
    if (m_IsArea && PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarChargeState::ChargeOn);
    }
}

void CSolarChargeScript::ChargeOn()
{
    if (!m_IsArea || !PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarChargeState::ChargeOff);
    }

    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::ChargedStart);
    }
}

void CSolarChargeScript::ChargeOff()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::OffWait);
    }
}

void CSolarChargeScript::ChargedStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::Decreases);
    }
}

void CSolarChargeScript::ChargedWait()
{
}

void CSolarChargeScript::Decrease()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::OffWaitStart);
    }
}

void CSolarChargeScript::OffWaitStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::OffWait);
    }
}

void CSolarChargeScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = true;
    }
}

void CSolarChargeScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = false;
    }
}

UINT CSolarChargeScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = ToWstring(m_MovingObjName);
    SaveWStringToFile(wStr, _File);

    MemoryByte += sizeof(wStr);

    return MemoryByte;
}

UINT CSolarChargeScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = {};
    LoadWStringFromFile(wStr, _File);

    m_MovingObjName = ToString(wStr);

    MemoryByte += sizeof(wStr);

    return MemoryByte;
}