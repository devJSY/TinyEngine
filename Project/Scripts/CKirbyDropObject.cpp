#include "pch.h"
#include "CKirbyDropObject.h"

CKirbyDropObject::CKirbyDropObject()
{
}

CKirbyDropObject::~CKirbyDropObject()
{
}

void CKirbyDropObject::tick()
{
    PLAYERFSM->GetPrevObject()->DropObject();

    // State Change
    switch (PLAYERFSM->GetPrevObjectIdx())
    {
    case ObjectCopyType::CONE:
    case ObjectCopyType::VENDING_MACHINE:
    case ObjectCopyType::LIGHT: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            if (PLAYERCTRL->IsGround())
            {
                ChangeState(L"IDLE");
            }
            else
            {
                ChangeState(L"JUMP_FALL");
            }
        }
    }
    break;
    }
}

void CKirbyDropObject::Enter()
{
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        PLAYERFSM->ChangeObjectCopy(ObjectCopyType::NONE);
    }

    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetPrevObject()->DropObjectEnter();
}

void CKirbyDropObject::Exit()
{
    PLAYERFSM->GetPrevObject()->DropObjectExit();
    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->ClearYPressedTime();
}