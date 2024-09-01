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
    PLAYERFSM->GetPrevObject()->DropObjectStart();

    // State Change
    switch (PLAYERFSM->GetPrevObjectIdx())
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
    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\DropObject.wav", 1, 50.f);

    PLAYERFSM->ChangeObjectCopy(ObjectCopyType::NONE);
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetPrevObject()->DropObjectStartEnter();
}

void CKirbyDropObjectStart::Exit()
{
    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetPrevObject()->DropObjectStartExit();
}