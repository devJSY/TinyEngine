#include "pch.h"
#include "CKirbyVacuum2.h"
#include "CKirbyVacuumCollider.h"

CKirbyVacuum2::CKirbyVacuum2()
    : m_SavedSpeed(0.f)
{
}

CKirbyVacuum2::~CKirbyVacuum2()
{
}

void CKirbyVacuum2::tick()
{
    // 키보드 입력이 없으면 해당 방향에서 멈추도록 설정
    if (PLAYERCTRL->GetInput().Length() == 0.f)
    {
        PLAYERCTRL->LockDirection();
    }
    else
    {
        PLAYERCTRL->UnlockDirection();
    }

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
        ChangeState(L"VACUUM2_RUN");
    }
}

void CKirbyVacuum2::Enter()
{
    // Vacuum Effect
    CGameObject* VacuumEffect = PLAYER->GetChildObject(L"Vacuum");
    if (VacuumEffect != nullptr)
    {
        VacuumEffect->SetActive(true);
    }

    PLAYER->Animator()->Play(ANIMPREFIX("SuperInhale"), true, false, 2.f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);
    // 회전 속도 제한
    m_SaveRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed / 4.f);


    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyVacuum2::Exit()
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
    PLAYERCTRL->UnlockDirection();

    // 회전 속도 복구
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}
