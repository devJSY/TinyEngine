#include "pch.h"
#include "CKirbyDropObjectStart.h"

CKirbyDropObjectStart::CKirbyDropObjectStart()
{
}

CKirbyDropObjectStart::~CKirbyDropObjectStart()
{
}

void CKirbyDropObjectStart::tick()
{
    PLAYERFSM->GetCurObject()->DropObjectStart();

    // State Change
    switch (PLAYERFSM->GetCurObjectIdx())
    {
    case ObjectCopyType::CONE:
    case ObjectCopyType::VENDING_MACHINE:
    case ObjectCopyType::LIGHT: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            ChangeState(L"DROP_OBJECT");
        }
    }
    break;
    }
}

void CKirbyDropObjectStart::Enter()
{
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurObject()->DropObjectStartEnter();
}

void CKirbyDropObjectStart::Exit()
{
    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetCurObject()->DropObjectStartExit();
}