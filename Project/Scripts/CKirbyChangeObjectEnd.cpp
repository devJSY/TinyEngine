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
    case ObjectCopyType::STAIR:
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
}

void CKirbyChangeObjectEnd::Exit()
{
    PLAYERFSM->GetCurObject()->ChangeObjectEndExit();
}