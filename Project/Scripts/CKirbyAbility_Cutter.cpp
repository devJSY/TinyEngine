#include "pch.h"
#include "CKirbyAbility_Cutter.h"

#include "CKirbyHatBlade.h"

CKirbyAbility_Cutter::CKirbyAbility_Cutter()
    : m_CutterBulletPrefab(nullptr)
{
}

CKirbyAbility_Cutter::~CKirbyAbility_Cutter()
{
}

void CKirbyAbility_Cutter::Attack()
{


    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyAbility_Cutter::AttackEnter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"MetalCutterThrow"), false, false, 1.5f);


    // 모자쪽에 Blade 사라지게하기
    CKirbyHatBlade* HatBladeScript = dynamic_cast<CKirbyHatBlade*>(PLAYERFSM->GetCurHatBlade()->GetScripts()[0]);

    if (HatBladeScript != nullptr)
    {
        HatBladeScript->Attack();
    }
    
    
    // Bullet 발사

    // Prefab 가져오기
    if (m_CutterBulletPrefab == nullptr)
    {
        m_CutterBulletPrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\MetalCutter.pref");
    }


    // 초기값 설정
    CGameObject* InstObj = m_CutterBulletPrefab->Instantiate();
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

void CKirbyAbility_Cutter::AttackExit()
{
}

void CKirbyAbility_Cutter::ChangeAbility()
{
}

void CKirbyAbility_Cutter::ChangeAbilityEnter()
{
    // Prefab 가져오기
    if (m_CutterBulletPrefab == nullptr)
    {
        m_CutterBulletPrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\MetalCutter.pref");
    }

    // 소켓에 모자 끼워주기
    CGameObject* pInstObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MetalHat.pref", L"prefab\\MetalHat.pref")->Instantiate();
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    // FSM에 모자 등록
    PLAYERFSM->SetCurHat(pInstObj);

    // HatBlade 따로 생성후 끼워주기
    pInstObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MetalHatBlade.pref", L"prefab\\MetalHatBlade.pref")->Instantiate();
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    // FSM에 HatBlade 저장
    PLAYERFSM->SetCurHatBlade(pInstObj);
    PLAYERFSM->SetBladeAttack(true);

    // 애니메이션 재생
    pInstObj->Animator()->Play(KIRBYANIM(L"BladeWait"));

}

void CKirbyAbility_Cutter::ChangeAbilityExit()
{
}
