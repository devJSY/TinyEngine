#include "pch.h"
#include "CKirbySlide.h"

CKirbySlide::CKirbySlide()
    : m_Speed(40.f)
    , m_Duration(0.3f)
    , m_Acc(0.f)
{
}

CKirbySlide::~CKirbySlide()
{
}

void CKirbySlide::tick()
{
    m_Acc += DT;

    if (m_Acc > m_Duration && PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"SLIDE_END");
    }
}

void CKirbySlide::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"Slide"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetGuard(true);
    PLAYERCTRL->SetFriction(0.07f);

    // Slide 방향 설정
    Vec3 PlayerFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    PlayerFront.Normalize();

    PLAYERCTRL->SetVelocity(PlayerFront * m_Speed);

    m_Acc = 0.f;
}

void CKirbySlide::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetGuard(false);
}
