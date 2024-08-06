#include "pch.h"
#include "CKirbyFinalCutterDrop.h"

CKirbyFinalCutterDrop::CKirbyFinalCutterDrop()
{
}

CKirbyFinalCutterDrop::~CKirbyFinalCutterDrop()
{
}

void CKirbyFinalCutterDrop::tick()
{
    if (PLAYERCTRL->IsGround())
    {
        ChangeState(L"FINALCUTTEREND");
    }
}

void CKirbyFinalCutterDrop::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("FinalCutterDrop"), true, false, 1.5f, 0);

    // 방향, 점프, 이동 Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    // 중력 조절
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(m_SaveGravity * 5.f);

    PLAYERCTRL->SetVelocity(Vec3(0.f, -50.f, 0.f));


    // Material 등록
    Ptr<CMaterial> CutterMaterial = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalCutter_BladeC.mtrl");

    PLAYERFSM->GetCurWeapon()->MeshRender()->SetMaterial(CutterMaterial, 0);

    // 머리의 Blade는 안보이게 한다
    PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(nullptr, 0);

    // 무적 상태
    PLAYERFSM->SetInvincible(true);

    PLAYERFSM->GetCurWeapon()->SetActive(true);
}

void CKirbyFinalCutterDrop::Exit()
{
    // Material 해제
    PLAYERFSM->GetCurWeapon()->MeshRender()->SetMaterial(nullptr, 0);

    // 방향, 점프, 이동 Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    // 중력 반환
    PLAYERCTRL->SetGravity(m_SaveGravity);

    // 머리의 Blade를 보이게 한다
    PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl"), 0);

    // 무적 상태
    PLAYERFSM->SetInvincible(false);

    PLAYERFSM->GetCurWeapon()->SetActive(true);
}
