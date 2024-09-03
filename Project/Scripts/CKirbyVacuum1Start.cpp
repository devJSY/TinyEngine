#include "pch.h"
#include "CKirbyVacuum1Start.h"

#include "CKirbyVacuumScript.h"

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
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        CKirbyVacuumScript* VacuumScript = VacuumEffect->GetScript<CKirbyVacuumScript>();
        VacuumScript->Reset();

        VacuumEffect->SetActive(true);
    }

    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Vacuum.wav", 2, KIRBY_EFFECTSOUND);

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
}