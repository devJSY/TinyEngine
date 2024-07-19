#include "pch.h"
#include "CKirbyDodge2.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>

CKirbyDodge2::CKirbyDodge2()
    : m_InitSpeed(15.f)
    , m_JumpPower(18.f)
    , m_DodgeSpeed(0.5f)
{
}

CKirbyDodge2::~CKirbyDodge2()
{
}

void CKirbyDodge2::tick()
{
    if (PLAYERCTRL->CharacterController()->IsGrounded())
    {

    }
    else if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
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
        ChangeState(L"IDLE");
    }
}

void CKirbyDodge2::Enter()
{
    Vec3 Front = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 Right = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

    switch (PLAYERFSM->GetDodgeType())
    {
    case DodgeType::NONE:
    case DodgeType::FRONT:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeFront2"), false, false, 1.5f);
        break;
    case DodgeType::RIGHT:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeRight2"), false, false, 1.5f);
        break;
    case DodgeType::BACK:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeBack2"), false, false, 1.5f);
        break;
    case DodgeType::LEFT:
        PLAYER->Animator()->Play(ANIMPREFIX("DodgeLeft2"), false, false, 1.5f);
        break;
    default:
        break;
    }

    Vec3 CurVel = PLAYERCTRL->GetVelocity();
    CurVel.y = 0.f;

    PLAYERCTRL->AddVelocity(CurVel * m_InitSpeed);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetFriction(2.f);
    PLAYERCTRL->SetGuard(true);
    PLAYERCTRL->SetGravity(-100.f);
    PLAYERCTRL->AddVelocity({0.f, m_JumpPower, 0.f});
}

void CKirbyDodge2::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetGuard(false);

    PLAYERCTRL->SetGravity(-20.f);
}
