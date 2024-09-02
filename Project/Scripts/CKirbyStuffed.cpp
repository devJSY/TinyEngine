#include "pch.h"
#include "CKirbyStuffed.h"

CKirbyStuffed::CKirbyStuffed()
{
}

CKirbyStuffed::~CKirbyStuffed()
{
}

void CKirbyStuffed::tick()
{
    // Change State
    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f)
        {
            ChangeState(L"STUFFED_RUN");
        }
        else
        {
            ChangeState(L"STUFFED_IDLE");
        }
    }
}

void CKirbyStuffed::Enter()
{
    GamePlayStatic::StopSound(L"sound\\wav\\HeroBasic\\Vacuum.wav");

    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Stuffed.wav", 1, KIRBY_EFFECTSOUND);
    PLAYER->Animator()->Play(ANIMPREFIX("Stuffed"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    PLAYERFSM->SetUnstuffReverse(false);
}

void CKirbyStuffed::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();

    PLAYERFSM->SetUnstuffReverse(true);
}