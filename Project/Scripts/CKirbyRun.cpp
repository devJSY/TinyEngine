#include "pch.h"
#include "CKirbyRun.h"

#define RUN_SPEED 10.f
#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyObject.h"
#include "CKirbyAbility.h"

CKirbyRun::CKirbyRun()
{
}

CKirbyRun::~CKirbyRun()
{
}

void CKirbyRun::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    
    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->Run();
    }
    else
    {
        //KirbyFSM->GetCurAbility()->Run();
    }

    // 기본적으로 수행해야 하는 동작
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
        ChangeState(L"ATTACK");
    else if (KEY_RELEASED_ARROW || KEY_NONE_ARROW)
        ChangeState(L"IDLE");
}

void CKirbyRun::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();

    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->RunEnter();
    }
    else
    {
        // KirbyFSM->GetCurObject()->IdleEnter();
        GetOwner()->Animator()->Play(KIRBYANIM(L"RunStart", false));
    }
}

void CKirbyRun::Exit()
{
    PLAY_CURSTATE(RunExit)
}
