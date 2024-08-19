#include "pch.h"
#include "CKirbyVacuum1Start.h"

CKirbyVacuum1Start::CKirbyVacuum1Start()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum1Start::~CKirbyVacuum1Start()
{
}

void CKirbyVacuum1Start::tick()
{
    if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        ChangeState(L"VACUUM1_END");
    }
    else if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f)
        {
            ChangeState(L"VACUUM1_RUN");
        }
        else
        {
            ChangeState(L"VACUUM1");
        }
    }
}

void CKirbyVacuum1Start::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("VacuumStart2"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);
}

void CKirbyVacuum1Start::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}