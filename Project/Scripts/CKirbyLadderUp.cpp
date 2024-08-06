#include "pch.h"
#include "CKirbyLadderUp.h"


CKirbyLadderUp::CKirbyLadderUp()
{
}

CKirbyLadderUp::~CKirbyLadderUp()
{
}

void CKirbyLadderUp::tick()
{
    // 이전에 프레임에 남아있는 VeloCity를 초기화
    PLAYERCTRL->VelocityCut(0.f);

    // 이동
    PLAYERCTRL->AddVelocity(Vec3(0.f, 5.f, 0.f));

    Vec3 InputWorld = PLAYERCTRL->GetInputWorld();
    Vec3 UpDir = PLAYERFSM->GetLadderUpSightDir();

    // 입력이 있는 경우
    if (InputWorld.Length() != 0.f)
    {
        if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
        {
            ChangeState(L"JUMP_START");
        }
        // 입력 방향이 Up방향 쪽인 아닌 경우 State를 Down상태로 바꾼다.
        else if(InputWorld.Dot(UpDir) < 0.f)
        {
            ChangeState(L"LADDER_DOWN");
        }
    }
     
    // 입력이 없는 경우 WAIT 상태로 바꾼다.
    else
    {
        ChangeState(L"LADDER_WAIT");
    }

}

void CKirbyLadderUp::Enter()
{
    PLAYERFSM->SetCollisionLadder(true);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("LadderUp"), true, false, 1.5f);

    // 중력 끄기
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);

    // Lock
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();

    /*Vec3 LadderPos = PLAYERFSM->GetLadderBottom();
    LadderPos.y = PLAYER->Transform()->GetWorldPos().y;

    PLAYER->Transform()->SetWorldPos(LadderPos);*/
}

void CKirbyLadderUp::Exit()
{
    PLAYERFSM->SetCollisionLadder(false);

    PLAYERCTRL->SetGravity(m_SaveGravity);

    // Unlock
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
}
