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
    PLAYERFSM->GetCurObject()->DropObject();

    // State Change
    switch (PLAYERFSM->GetCurObjectIdx())
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
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurObject()->DropObjectEnter();
}

void CKirbyDropObject::Exit()
{
    PLAYERFSM->GetCurObject()->DropObjectExit();
    PLAYERFSM->SetInvincible(false);

    PLAYERFSM->ClearYPressedTime();
    PLAYERFSM->ChangeObjectCopy(ObjectCopyType::NONE);
}