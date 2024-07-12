#include "pch.h"
#include "CKirbyGuard.h"

#include <Engine/CRenderMgr.h>

CKirbyGuard::CKirbyGuard()
{
}

CKirbyGuard::~CKirbyGuard()
{
}
void CKirbyGuard::tick()
{
    if (KEY_TAP_ARROW)
    {
        CCamera* MainCam = CRenderMgr::GetInst()->GetMainCamera();

        // �Է� ����
        Vec3 InputDir = PLAYERCTRL->GetInput();

        Vec3 Front = MainCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 Right = MainCam->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

        // y�� �̵��� ������Ʈ�� ���ؼ� ó��
        Front.y = 0.f;
        Right.y = 0.f;

        Vec3 MoveDir = {0.f, 0.f, 0.f};

        MoveDir = XMVectorAdd(XMVectorScale(Front, InputDir.z), XMVectorScale(Right, InputDir.x));
        MoveDir.Normalize();

        Vec3 KirbyFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        KirbyFront.y = 0.f;
        KirbyFront.Normalize();

        if (MoveDir.Length() != 0.f && MainCam != nullptr)
        {

            float ResultDot = MoveDir.Dot(KirbyFront);

            // World���� CamFront�� 45�� �̳��� ������ DODGE_FRONT
            if (ResultDot >= cosf(XM_PI / 4.f))
            {
                PLAYERFSM->SetDodgeType(DodgeType::FRONT);
            }

            else if (ResultDot <= cosf(3.f * XM_PI / 4.f))
            {
                PLAYERFSM->SetDodgeType(DodgeType::BACK);
            }

            else
            {
                if (KirbyFront.Cross(MoveDir).y > 0.f)
                {
                    PLAYERFSM->SetDodgeType(DodgeType::RIGHT);
                }
                else
                {
                    PLAYERFSM->SetDodgeType(DodgeType::LEFT);
                }
            }
        }

        ChangeState(L"DODGE_START");
    }

    else if (KEY_TAP(KEY_JUMP))
    {
        ChangeState(L"SLIDE_START");
    }

    else if (KEY_RELEASED(KEY_GUARD) || KEY_NONE(KEY_GUARD))
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyGuard::Enter()
{
    // �ִϸ��̼� ���
    PLAYER->Animator()->Play(KIRBYANIM(L"Guard"), true, false, 1.f, 0.2);
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // ����, ����, �̵� Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetFriction(0.1f);
    PLAYERCTRL->SetGuard(true);

    // DodgeType reset
    PLAYERFSM->SetDodgeType(DodgeType::NONE);
}

void CKirbyGuard::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    // ����, ����, �̵� Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetGuard(false);
}