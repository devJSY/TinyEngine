#include "pch.h"
#include "CKirbyLadderDown.h"

CKirbyLadderDown::CKirbyLadderDown()
{
}

CKirbyLadderDown::~CKirbyLadderDown()
{
}

void CKirbyLadderDown::tick()
{
    // 이전에 프레임에 남아있는 VeloCity를 초기화
    PLAYERCTRL->VelocityCut(0.f);

    Vec3 InputWorld = PLAYERCTRL->GetInputWorld();
    Vec3 DownDir = PLAYERFSM->GetLadderDownSightDir();

    switch (m_DownState)
    {
    case DOWN_STATE::SLOW: {
        // 이동
        PLAYERCTRL->AddVelocity(Vec3(0.f, -5.f, 0.f));

        // 입력이 있는 경우
        if (InputWorld.Length() != 0.f)
        {
            // 입력 방향이 Down방향 쪽인 아닌 경우 State를 UP상태로 바꾼다.
            if (InputWorld.Dot(DownDir) < 0.f)
            {
                ChangeState(L"LADDER_UP");
            }
        }
        // 입력이 없는 경우 WAIT 상태로 바꾼다.
        else
        {
            ChangeState(L"LADDER_WAIT");
        }

        if (PLAYER->Animator()->IsFinish())
        {
            m_DownState = DOWN_STATE::FAST;
            PLAYER->Animator()->Play(ANIMPREFIX("LadderDown"), true, false, 1.5f);
        }
    }
    break;

    case DOWN_STATE::FAST: {
        // 이동
        PLAYERCTRL->AddVelocity(Vec3(0.f, -10.f, 0.f));

        // 입력이 있는 경우
        if (InputWorld.Length() != 0.f)
        {
            // 입력 방향이 Down방향 쪽인 아닌 경우 State를 UP상태로 바꾼다.
            if (InputWorld.Dot(DownDir) < 0.f)
            {
                ChangeState(L"LADDER_UP");
            }
        }
        // 입력이 없는 경우 WAITSTART 상태로 바꾼다.
        else
        {
            ChangeState(L"LADDER_WAITSTART");
        }
    }
    break;
    }

    // 땅에 닿는다면 Idle 상태로 변경한다
    if (PLAYERCTRL->IsGround())
    {
        ChangeState(L"IDLE");
    }

}

void CKirbyLadderDown::Enter()
{
    PLAYERFSM->SetCollisionLadder(true);

    m_DownState = DOWN_STATE::SLOW;

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("LadderUp"), false, true, 1.5f);

    // 중력 끄기
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);

    // Lock
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
}

void CKirbyLadderDown::Exit()
{
    PLAYERFSM->SetCollisionLadder(false);

    PLAYERCTRL->SetGravity(m_SaveGravity);

    // Unlock
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
}
