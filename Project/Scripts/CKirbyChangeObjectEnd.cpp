#include "pch.h"
#include "CKirbyChangeObjectEnd.h"

CKirbyChangeObjectEnd::CKirbyChangeObjectEnd()
{
}

CKirbyChangeObjectEnd::~CKirbyChangeObjectEnd()
{
}

void CKirbyChangeObjectEnd::tick()
{
    PLAYERFSM->GetCurObject()->ChangeObjectEnd();

    // State Change
    switch (PLAYERFSM->GetCurObjectIdx())
    {
    case ObjectCopyType::CONE:
    case ObjectCopyType::VENDING_MACHINE:
    case ObjectCopyType::LIGHT: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            ChangeState(L"IDLE");
        }
    }
        break;
    }
}

void CKirbyChangeObjectEnd::Enter()
{
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->GetCurObject()->ChangeObjectEndEnter();

    // 변신 중일 땐 커비가 이미시브 효과를 받지않도록 한다.
    PLAYERFSM->SetSkrr(true);
}

void CKirbyChangeObjectEnd::Exit()
{
    PLAYERFSM->GetCurObject()->ChangeObjectEndExit();

    // Emissive를 다시 받도록 수정
    PLAYERFSM->SetSkrr(false);
}