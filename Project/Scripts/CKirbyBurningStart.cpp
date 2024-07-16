#include "pch.h"
#include "CKirbyBurningStart.h"

CKirbyBurningStart::CKirbyBurningStart()
    : m_SaveRotSpeed(0.f)
    , m_SaveSpeed(0.f)
{
}

CKirbyBurningStart::~CKirbyBurningStart()
{
}


void CKirbyBurningStart::tick()
{
    // 전체 글라이딩 시간을 위해 FSM에 Gliding 시간을 더해준다.
    PLAYERFSM->AddGlidingTime(DT);

    // 중력값 조절
    float CurGravity = PLAYERCTRL->GetGravity();
    if (CurGravity < 0)
    {
        CurGravity += 100.f * DT;
        PLAYERCTRL->SetGravity(CurGravity);
    }


    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"BURNING");
    }
}

void CKirbyBurningStart::Enter()
{
    // 애니메이션 재생
    //PLAYER->Animator()->Play(ANIMPREFIX(L"BurningStart"), false, false, 1.5f);

    PLAYER->Animator()->Play(ANIMPREFIX(L"DodgeFront1"), false, false, 1.5f);

    // 글라이딩 시간 초기화
    PLAYERFSM->ClearGlidingTime();

    // Movement
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetForwardMode(true);
    m_SaveRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetRotSpeed(0.5f);

    m_SaveSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(13.f);

    PLAYERCTRL->ClearVelocityY();
}

void CKirbyBurningStart::Exit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetForwardMode(false);
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);
    PLAYERCTRL->SetSpeed(m_SaveSpeed);
}
