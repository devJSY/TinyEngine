#include "pch.h"
#include "CKirbyDodge1.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>

CKirbyDodge1::CKirbyDodge1()
    : m_InitSpeed(12.f)
    , m_JumpPower(18.f)
    , m_DodgeSpeed(0.1f)
{
}

CKirbyDodge1::~CKirbyDodge1()
{
}

void CKirbyDodge1::tick()
{
    if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
    {
        CCamera* MainCam = CRenderMgr::GetInst()->GetMainCamera();

        // 입력 방향
        Vec3 InputDir = PLAYERCTRL->GetInput();

        Vec3 Front = MainCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 Right = MainCam->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

        // y축 이동은 컴포넌트에 의해서 처리
        Front.y = 0.f;
        Right.y = 0.f;

        Vec3 MoveDir = {0.f, 0.f, 0.f};

        MoveDir = XMVectorAdd(XMVectorScale(Front, InputDir.z), XMVectorScale(Right, InputDir.x));
        MoveDir.Normalize();

        PLAYERCTRL->AddVelocity(MoveDir * m_DodgeSpeed);
    }


    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"DODGE2");
        }
        else
        {
            if (KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"GUARD");
            }
            else
            {
                ChangeState(L"JUMP_FALL");
            }
        }

    }
}

void CKirbyDodge1::Enter()
{
    Vec3 Front = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 Right = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

    switch (PLAYERFSM->GetDodgeType())
    {
    case DodgeType::NONE:
    case DodgeType::FRONT:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeFront1"), false, false, 1.5f);
        PLAYERCTRL->AddVelocity(Front * m_InitSpeed);
        break;
    case DodgeType::RIGHT:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeRight1"), false, false, 1.5f);
        PLAYERCTRL->AddVelocity(Right * m_InitSpeed);
        break;
    case DodgeType::BACK:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeBack1"), false, false, 1.5f);
        PLAYERCTRL->AddVelocity(-Front * m_InitSpeed);
        break;
    case DodgeType::LEFT:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeLeft1"), false, false, 1.5f);
        PLAYERCTRL->AddVelocity(-Right * m_InitSpeed);
        break;
    default:
        break;
    }

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetFriction(2.5f);
    PLAYERCTRL->SetFrictionMode(true);
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(-100.f);
    PLAYERCTRL->AddVelocity({0.f, m_JumpPower, 0.f});
}

void CKirbyDodge1::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();


    PLAYERCTRL->SetGravity(m_SaveGravity);
    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetFrictionMode(false);
}
