#include "pch.h"
#include "CKirbyDodgeStart.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>

CKirbyDodgeStart::CKirbyDodgeStart()
{
}

CKirbyDodgeStart::~CKirbyDodgeStart()
{
}

void CKirbyDodgeStart::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        CCamera* MainCam = CRenderMgr::GetInst()->GetMainCamera();

        // 입력 방향 (World 좌표계)
        Vec3 MoveDir = PLAYERCTRL->GetMoveDir();

        if (MoveDir.Length() != 0.f && MainCam != nullptr)
        {
            Vec3 CamFront = MainCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            CamFront.y = 0.f;
            CamFront.Normalize();

            float ResultDot = MoveDir.Dot(CamFront);

            // World기준 CamFront와 45도 이내의 각도는 DODGE_FRONT
            if (ResultDot >= cosf(XM_PI / 4.f))
            {
                ChangeState(L"DODGE_FRONT1");
            }

            else if (ResultDot <= cosf(3.f * XM_PI / 4.f))
            {
                ChangeState(L"DODGE_BACK1");
            }

            else
            {
                if (CamFront.Cross(MoveDir).y > 0.f)
                {
                    ChangeState(L"DODGE_RIGHT1");
                }
                else
                {
                    ChangeState(L"DODGE_LEFT1");
                }
            }
        }
    }
}

void CKirbyDodgeStart::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeStart"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeStart::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
