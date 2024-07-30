#include "pch.h"
#include "CKirbySlide.h"

CKirbySlide::CKirbySlide()
    : m_Speed(150.f)
    , m_Duration(0.5f)
    , m_Acc(0.f)
{
}

CKirbySlide::~CKirbySlide()
{
}

void CKirbySlide::tick()
{
    m_Acc += DT;

    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->Slide();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::FIRE:
    case AbilityCopyType::CUTTER: {
        if (m_Acc > m_Duration)
        {
            ChangeState(L"SLIDE_END");
        }
    }
    break;
    case AbilityCopyType::SWORD: {
        if (KEY_TAP(KEY_JUMP) || KEY_PRESSED(KEY_JUMP))
        {
            if (m_Acc >= 0.2f)
            {
                ChangeState(L"SLIDE_ATTACK");
            }
        }
        else if (m_Acc > m_Duration)
        {
            ChangeState(L"SLIDE_END");
        }
    }
    break;
    }
}

void CKirbySlide::Enter()
{
    // 애니메이션 재생
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideEnter();

    // Controller 세팅
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetGuard(true);
    PLAYERCTRL->SetFriction(1.5f);

    // Slide 방향 설정
    Vec3 PlayerFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT).Normalize();
    PLAYERCTRL->SetVelocity(PlayerFront * m_Speed);

    // 커비 표정 바꾸기
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    m_Acc = 0.f;
}

void CKirbySlide::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideExit();

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetGuard(false);
    PLAYERCTRL->SetFriction(0.f);

    // 커비 표정 바꾸기
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
}
