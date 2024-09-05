#include "pch.h"
#include "CKirbyVacuum1Run.h"
#include "CKirbyVacuumCollider.h"

CKirbyVacuum1Run::CKirbyVacuum1Run()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum1Run::~CKirbyVacuum1Run()
{
}

void CKirbyVacuum1Run::tick()
{
    if (PLAYERFSM->IsStuffed() && PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::NORMAL)
    {
        ChangeState(L"STUFFED");
    }
    else if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        ChangeState(L"VACUUM1_END");
    }
    else if (PLAYERCTRL->GetInput().Length() == 0.f)
    {
        ChangeState(L"VACUUM1");
    }
    else if (PLAYERFSM->GetChargeAccTime() > PLAYERFSM->GetVacuum1MaxTime())
    {
        ChangeState(L"VACUUM2_RUN");
    }
}

void CKirbyVacuum1Run::Enter()
{
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(true);
    }


    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Vacuum.wav", 2, KIRBY_EFFECTSOUND, true, false);

    PLAYER->Animator()->Play(ANIMPREFIX("VacuumWalk"), true, false, 2.f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyVacuum1Run::Exit()
{
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(false);
    }

    GamePlayStatic::PauseSound(L"sound\\wav\\HeroBasic\\Vacuum.wav");

    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}