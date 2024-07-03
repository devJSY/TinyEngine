#include "pch.h"
#include "CKirbyStuffedLanding.h"

CKirbyStuffedLanding::CKirbyStuffedLanding()
{
}

CKirbyStuffedLanding::~CKirbyStuffedLanding()
{
}

void CKirbyStuffedLanding::tick()
{
    // PLAY_CURSTATE(StuffedLanding)

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"STUFFED_IDLE");
    }
}

void CKirbyStuffedLanding::Enter()
{
    // PLAY_CURSTATE(StuffedLanding)
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedLanding"), false);
}

void CKirbyStuffedLanding::Exit()
{
    // PLAY_CURSTATE(StuffedLanding)
}