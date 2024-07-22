#include "pch.h"
#include "CKirbyChangeObject.h"

CKirbyChangeObject::CKirbyChangeObject()
{
}

CKirbyChangeObject::~CKirbyChangeObject()
{
}

void CKirbyChangeObject::tick()
{
    PLAYERFSM->GetCurObject()->ChangeObject();

    // State Change
    switch (PLAYERFSM->GetCurObjectIdx())
    {
    case ObjectCopyType::CONE:
    case ObjectCopyType::STAIR:
    case ObjectCopyType::LIGHT: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            ChangeState(L"CHANGE_OBJECT_END");
        }
    }
        break;
    }
}

void CKirbyChangeObject::Enter()
{
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->GetCurObject()->ChangeObjectEnter();
}

void CKirbyChangeObject::Exit()
{
    PLAYERFSM->GetCurObject()->ChangeObjectExit();
}
