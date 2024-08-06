#include "pch.h"
#include "CKirbyFinalCutterRise.h"

CKirbyFinalCutterRise::CKirbyFinalCutterRise()
    : m_SaveGravity(0.f)
{
}

CKirbyFinalCutterRise::~CKirbyFinalCutterRise()
{
}


void CKirbyFinalCutterRise::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"FINALCUTTERDROP");
    }
}

void CKirbyFinalCutterRise::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("FinalCutterRise"), false, false, 1.5f, 0);

    // 방향, 이동 Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();

    // 중력 조절
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(m_SaveGravity * 1.5f);

    //점프력 조절
    m_SaveJumpPower = PLAYERCTRL->GetJumpPower();
    PLAYERCTRL->SetJumpPower(m_SaveJumpPower * 2.f);

    // JUMP
    PLAYERCTRL->Jump();
    
    // Material 등록
    Ptr<CMaterial> CutterMaterial = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalCutter_BladeC.mtrl");

    PLAYERFSM->GetCurWeapon()->MeshRender()->SetMaterial(CutterMaterial, 0);

    // 머리의 Blade는 안보이게 한다
    PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(nullptr, 0);

    // 무적 상태
    PLAYERFSM->SetInvincible(true);

    // 무기 Active
    PLAYERFSM->GetCurWeapon()->SetActive(true);

}

void CKirbyFinalCutterRise::Exit()
{
    // 방향, 이동 Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();

    // 중력 반환
    PLAYERCTRL->SetGravity(m_SaveGravity);

    // 점프력 반환
    PLAYERCTRL->SetJumpPower(m_SaveJumpPower);

    // Material 해제
    PLAYERFSM->GetCurWeapon()->MeshRender()->SetMaterial(nullptr, 0);

    // 머리의 Blade를 보이게 한다
    PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl"), 0);

    // 무적 상태
    PLAYERFSM->SetInvincible(false);

    PLAYERFSM->GetCurWeapon()->SetActive(false);
}
