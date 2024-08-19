#include "pch.h"
#include "CKirbyVacuum1.h"
#include "CKirbyVacuumCollider.h"

CKirbyVacuum1::CKirbyVacuum1()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum1::~CKirbyVacuum1()
{
}

void CKirbyVacuum1::tick()
{
    if (PLAYERFSM->IsStuffed() && PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::NORMAL)
    {
        ChangeState(L"STUFFED");
    }
    else if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        ChangeState(L"VACUUM1_END");
    }
    else if (PLAYERCTRL->GetInput().Length() != 0.f)
    {
        ChangeState(L"VACUUM1_RUN");
    }
    else if (PLAYERFSM->GetChargeAccTime() > PLAYERFSM->GetVacuum1MaxTime())
    {
        ChangeState(L"VACUUM2_START");
    }
}

void CKirbyVacuum1::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Vacuum"), true, false, 2.f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(4.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyVacuum1::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}