#include "pch.h"
#include "CKirbyVacuum1End.h"

CKirbyVacuum1End::CKirbyVacuum1End()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum1End::~CKirbyVacuum1End()
{
}

void CKirbyVacuum1End::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"IDLE");
        }
        else
        {
            ChangeState(L"JUMP_FALL");
        }
    }
}

void CKirbyVacuum1End::Enter()
{
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(false);
    }

    GamePlayStatic::PauseSound(L"sound\\wav\\HeroBasic\\Vacuum.wav");

    PLAYER->Animator()->Play(ANIMPREFIX("VacuumEnd"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);
}

void CKirbyVacuum1End::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}