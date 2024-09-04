#include "pch.h"
#include "CKirbyVacuum2Start.h"
#include "CKirbyVacuumCollider.h"

CKirbyVacuum2Start::CKirbyVacuum2Start()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum2Start::~CKirbyVacuum2Start()
{
}

void CKirbyVacuum2Start::tick()
{
    if (PLAYERFSM->IsStuffed() && PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::NORMAL)
    {
        ChangeState(L"STUFFED");
    }
    else if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        ChangeState(L"VACUUM1_END");
    }
    else if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f)
        {
            ChangeState(L"VACUUM2_RUN");
        }
        else
        {
            ChangeState(L"VACUUM2");
        }
    }
}

void CKirbyVacuum2Start::Enter()
{
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(true);
    }

    PLAYER->Animator()->Play(ANIMPREFIX("SuperInhaleStart"), false);
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyVacuum2Start::Exit()
{ 
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(false);
    }

    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}