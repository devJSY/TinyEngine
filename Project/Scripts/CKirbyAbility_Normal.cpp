#include "pch.h"
#include "CKirbyAbility_Normal.h"
#include "CKirbyBulletScript.h"
#include "CKirbyMoveController.h"

#define BULLET_SPEED 5.f

CKirbyAbility_Normal::CKirbyAbility_Normal()
    : m_bFrmEnter(true)
{
    m_Charge1Time = 3.f;
}

CKirbyAbility_Normal::~CKirbyAbility_Normal()
{
}

// ===============
// RUN
// ===============
void CKirbyAbility_Normal::Run()
{
}

void CKirbyAbility_Normal::RunEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Run"));
}

void CKirbyAbility_Normal::RunExit()
{
}

// start
void CKirbyAbility_Normal::RunStart()
{
}

void CKirbyAbility_Normal::RunStartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"RunStart"), false);
}

void CKirbyAbility_Normal::RunStartExit()
{
}

// ===============
// Attack
// ===============
// 머금은 물체 뱉기
// - Stuffed일 때만 들어옴

void CKirbyAbility_Normal::Attack()
{
    if (GET_CURCLIP_FRM == 3 && m_bFrmEnter)
    {
        // @TODO Material 이름으로 받아오기
        PLAYER->GetRenderComponent()->SetMaterial(nullptr, 1);
        m_bFrmEnter = false;

        // fire bullet
        Ptr<CPrefab> BulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\BulletStar.pref", L"prefab\\BulletStar.pref");
        if (nullptr != BulletPref)
        {
            CGameObject* BulletInst = BulletPref->Instantiate();
            Vec3 InitPos = PLAYER->Transform()->GetWorldPos() + PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 2.f;

            BulletInst->Transform()->SetRelativePos(InitPos);
            GamePlayStatic::SpawnGameObject(BulletInst, 0);

            CKirbyBulletScript* bulletScript = BulletInst->GetScript<CKirbyBulletScript>();
            if (nullptr != bulletScript)
            {
                bulletScript->SetInitVelocity(PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * BULLET_SPEED);
            }
        }
    }
}

void CKirbyAbility_Normal::AttackEnter()
{
    if (PLAYER->CharacterController()->IsGrounded())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Spit"), false);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"SpitAir"), false);
    }

    m_bFrmEnter = true;
}

void CKirbyAbility_Normal::AttackExit()
{
}

// ===============
// Charge Attack 1
// ===============
// 흡입하기1

void CKirbyAbility_Normal::AttackCharge1()
{
}

void CKirbyAbility_Normal::AttackCharge1Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Vacuum"));
}

void CKirbyAbility_Normal::AttackCharge1Exit()
{
}

// start
void CKirbyAbility_Normal::AttackCharge1Start()
{
}

void CKirbyAbility_Normal::AttackCharge1StartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumStart2"), false);

    // @TODO Material 이름으로 받아오기 & material key값 변경
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 6);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 0);
    // @TODO 속도조절
}

void CKirbyAbility_Normal::AttackCharge1StartExit()
{
}

// end
void CKirbyAbility_Normal::AttackCharge1End()
{
}

void CKirbyAbility_Normal::AttackCharge1EndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumEnd"), false);

    // @TODO Material 이름으로 받아오기 & material key값 변경
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 0);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 6);
    // @TODO 속도조절
}

void CKirbyAbility_Normal::AttackCharge1EndExit()
{
}

// Run
void CKirbyAbility_Normal::AttackCharge1Run()
{
}

void CKirbyAbility_Normal::AttackCharge1RunEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumWalk"));
}

void CKirbyAbility_Normal::AttackCharge1RunExit()
{
}

// ===============
// Charge Attack 2
// ===============
// 흡입하기2
void CKirbyAbility_Normal::AttackCharge2()
{
}

void CKirbyAbility_Normal::AttackCharge2Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumHustleLv2"));

    // @TODO Material 이름으로 받아오기 & material key값 변경
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 0);
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 6);
    // @TODO 속도조절
}

void CKirbyAbility_Normal::AttackCharge2Exit()
{
}