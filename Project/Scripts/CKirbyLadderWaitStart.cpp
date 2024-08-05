#include "pch.h"
#include "CKirbyLadderWaitStart.h"


CKirbyLadderWaitStart::CKirbyLadderWaitStart()
{
}

CKirbyLadderWaitStart::~CKirbyLadderWaitStart()
{
}


void CKirbyLadderWaitStart::tick()
{
    // 이전에 프레임에 남아있는 VeloCity를 초기화
    PLAYERCTRL->VelocityCut(0.f);

    Vec3 InputWorld = PLAYERCTRL->GetInputWorld();
    Vec3 UpDir = PLAYERFSM->GetLadderUpSightDir();

    // 입력이 있는 경우
    if (InputWorld.Length() != 0.f)
    {
        // 입력 방향이 Up방향 쪽인 경우 State를 사다리를 올라가는 상태로 바꾼다.
        if (InputWorld.Dot(UpDir) > 0.f)
        {
            ChangeState(L"LADDER_UP");
        }
        else
        {
            ChangeState(L"LADDER_DOWN");
        }
    }
    else if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"LADDER_WAIT");
    }
}

void CKirbyLadderWaitStart::Enter()
{
    PLAYERFSM->SetCollisionLadder(true);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("LadderWaitStart"), false, false, 1.5f);

    // 중력 끄기
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);

    // Lock
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
}

void CKirbyLadderWaitStart::Exit()
{
    PLAYERFSM->SetCollisionLadder(false);

    PLAYERCTRL->SetGravity(m_SaveGravity);

    // Unlock
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
}
