#include "pch.h"
#include "CKirbyVacuum2Run.h"
#include "CKirbyVacuumCollider.h"

CKirbyVacuum2Run::CKirbyVacuum2Run()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum2Run::~CKirbyVacuum2Run()
{
}

void CKirbyVacuum2Run::tick()
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
        ChangeState(L"VACUUM2");
    }
}

void CKirbyVacuum2Run::Enter()
{
    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Vacuum.wav", 2, KIRBY_EFFECTSOUND, true, false);

    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(true);
    }

    PLAYER->Animator()->Play(ANIMPREFIX("SuperInhaleWalk"), true, false, 2.f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyVacuum2Run::Exit()
{
    GamePlayStatic::PauseSound(L"sound\\wav\\HeroBasic\\Vacuum.wav");

    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(false);
    }

    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}