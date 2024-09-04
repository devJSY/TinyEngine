#include "pch.h"
#include "CKirbyBurningWallEnd.h"

CKirbyBurningWallEnd::CKirbyBurningWallEnd()
    : m_JumpPower(8.f)
    , m_InitSpeed(9.f)
    , m_StateEnter(true)
{
}

CKirbyBurningWallEnd::~CKirbyBurningWallEnd()
{
}

void CKirbyBurningWallEnd::tick()
{

    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"IDLE");
        }
        else
        {
            ChangeState(L"JUMP_FALL");
        }
    }
}

void CKirbyBurningWallEnd::Enter()
{
    Ptr<CPrefab> FireWallSmokeSpawner = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireWallSmokeSpawner.pref");

    if (FireWallSmokeSpawner.Get())
    {
        CGameObject* FireWallSmokeSpawnerObj = FireWallSmokeSpawner->Instantiate();
        FireWallSmokeSpawnerObj->Transform()->SetWorldPos(PLAYER->Transform()->GetWorldPos());

        GamePlayStatic::SpawnGameObject(FireWallSmokeSpawnerObj, FireWallSmokeSpawnerObj->GetLayerIdx());
    }

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("BurningWallEnd"), false, false, 2.5f);

    Vec3 KnockBackDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    KnockBackDir.y = 0.f;
    KnockBackDir.Normalize();

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetFrictionMode(true);

    PLAYERCTRL->ClearVelocityHorizontal();

    PLAYERCTRL->AddVelocity({0.f, m_JumpPower, 0.f});
    PLAYERCTRL->AddVelocity(-KnockBackDir * m_InitSpeed);

    PLAYERCTRL->SetGravity(-35.f);

    m_StateEnter = true;
}

void CKirbyBurningWallEnd::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetFrictionMode(false);

    PLAYERCTRL->SetGravity(PLAYERCTRL->GetInitGravity());
}
