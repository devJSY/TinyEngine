#include "pch.h"
#include "CKirbyGuardRun.h"

CKirbyGuardRun::CKirbyGuardRun()
    : m_PlayTime(2.f)
{
}

CKirbyGuardRun::~CKirbyGuardRun()
{
}

void CKirbyGuardRun::tick()
{
    m_PlayTime -= DT;

    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->GuardRun();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::FIRE:
    case AbilityCopyType::RANGER:
        break;
    case AbilityCopyType::SWORD:{
        if (m_PlayTime < 0.f || PLAYERCTRL->GetInput().Length() == 0.f)
        {
            ChangeState(L"GUARD");
        }
    }
        break;
    }
}

void CKirbyGuardRun::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->GuardRunEnter();

    m_PlayTime = 2.f;
}

void CKirbyGuardRun::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->GuardRunExit();
}