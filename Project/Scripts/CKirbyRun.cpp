#include "pch.h"
#include "CKirbyRun.h"

#define RUN_SPEED 10.f

CKirbyRun::CKirbyRun()
{
}

CKirbyRun::~CKirbyRun()
{
}

void CKirbyRun::tick()
{
    // Common Run
    Vec3 NewPos = GetOwner()->Transform()->GetRelativePos();

    if (KEY_TAP(KEY::LEFT) || KEY_PRESSED(KEY::LEFT))
        NewPos.x -= 10.f * DT;
    if (KEY_TAP(KEY::RIGHT) || KEY_PRESSED(KEY::RIGHT))
        NewPos.x += 10.f * DT;
    if (KEY_TAP(KEY::UP) || KEY_PRESSED(KEY::UP))
        NewPos.z += 10.f * DT;
    if (KEY_TAP(KEY::DOWN) || KEY_PRESSED(KEY::DOWN))
        NewPos.z -= 10.f * DT;

    GetOwner()->Transform()->SetRelativePos(NewPos);

    // Change State
    if (KEY_TAP(KEY::Q))
    {
        ChangeState(L"ATTACK");
    }
    else if (KEY_RELEASED_ARROW || KEY_NONE_ARROW)
    {
        ChangeState(L"IDLE");
    }

    // Per State
    PLAY_CURSTATE(Run)
}

void CKirbyRun::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();

    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->RunEnter();
        GetOwner()->Animator()->Play(KIRBYANIM(L"Run"));
    }
    else
    {
        KirbyFSM->GetCurAbility()->RunEnter();
    }
}

void CKirbyRun::Exit()
{
    PLAY_CURSTATE(RunExit)
}