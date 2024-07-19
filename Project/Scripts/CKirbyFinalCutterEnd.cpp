#include "pch.h"
#include "CKirbyFinalCutterEnd.h"

#include "CKirbyHatBlade.h"

CKirbyFinalCutterEnd::CKirbyFinalCutterEnd()
{
}

CKirbyFinalCutterEnd::~CKirbyFinalCutterEnd()
{
}


void CKirbyFinalCutterEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"FINALCUTTERENDAFTER");
    }
}

void CKirbyFinalCutterEnd::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("FinalCutterEnd"), false, false, 1.5f, 0);

    // 방향, 점프, 이동 Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    // Material 등록
    Ptr<CMaterial> CutterMaterial = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\MetalCutter_BladeC.mtrl");

    PLAYERFSM->GetCurWeapon()->MeshRender()->SetMaterial(CutterMaterial, 0);

    
    // 무적 상태
    PLAYERFSM->SetInvincible(true);

        // 모자쪽에 Blade 사라지게하기
    CKirbyHatBlade* HatBladeScript = dynamic_cast<CKirbyHatBlade*>(PLAYERFSM->GetCurHatBlade()->GetScripts()[0]);

    if (HatBladeScript != nullptr)
    {
        HatBladeScript->Attack();
    }

    // Bullet 발사
    // Prefab 가져오기
    Ptr<CPrefab> CutterBulletPrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\MetalCutter.pref");

    // 초기값 설정
    CGameObject* InstObj = CutterBulletPrefab->Instantiate();
    Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();
    Vec3 KirbyFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    KirbyFront.y = 0.f;
    KirbyFront.Normalize();

    Vec3 Offset = Vec3(0.f, 50.f, 0.f);

    // Transform
    InstObj->Transform()->SetWorldPos(KirbyPos + Offset);
    InstObj->Transform()->SetDirection(-KirbyFront);

    GamePlayStatic::SpawnGameObject(InstObj, InstObj->GetLayerIdx());
}

void CKirbyFinalCutterEnd::Exit()
{
    // Material 해제
    PLAYERFSM->GetCurWeapon()->MeshRender()->SetMaterial(nullptr, 0);

    // 방향, 점프, 이동 Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    // 무적 상태
    PLAYERFSM->SetInvincible(false);
}
