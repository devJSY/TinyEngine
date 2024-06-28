#include "pch.h"
#include "CKirbyAbility_Normal.h"
#include "CKirbyBulletScript.h"

#define BULLET_SPEED 5.f


// 이거 Ability쪽에 박아도 되나?
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CKirbyAbility_Normal::CKirbyAbility_Normal()
{
	// 사용 메쉬 정의
	// 사용 애니메이션 정의
}

CKirbyAbility_Normal::~CKirbyAbility_Normal()
{
}

// ===============
// RUN
// ===============
void CKirbyAbility_Normal::RunStart()
{
}

void CKirbyAbility_Normal::RunStartEnter()
{
    if (PLAYERFSM->IsStuffed())
        PLAYER->Animator()->Play(KIRBYANIM(L"StuffedRun"), false);
    else
        PLAYER->Animator()->Play(KIRBYANIM(L"RunStart"), false);
}

void CKirbyAbility_Normal::RunStartExit()
{
}

void CKirbyAbility_Normal::Run()
{
}

void CKirbyAbility_Normal::RunEnter()
{
}

void CKirbyAbility_Normal::RunExit()
{
}

// ===============
// Attack
// ===============
// 머금은 물체 뱉기
void CKirbyAbility_Normal::Attack()
{
}

void CKirbyAbility_Normal::AttackEnter()
{
    // ==============
    // case: Vacuum
    // ==============
    if (!PLAYERFSM->IsStuffed())
    {
        ChangeState(L"ATTACK_CHARGE1");
        return;
    }

    // ==============
    // case: Shoot
    // ==============
    CPlayerMgr::GetPlayer()->Transform()->SetRelativeScale(Vec3(1, 1, 1));
    PLAYERFSM->SetStuffed(false);

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

    ChangeState(L"IDLE");
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
    PLAYERFSM->SetStuffed(true);
    CPlayerMgr::GetPlayer()->Transform()->SetRelativeScale(Vec3(2, 2, 2));
}

void CKirbyAbility_Normal::AttackCharge1Exit()
{
    if (!PLAYERFSM->IsStuffed())
        CPlayerMgr::GetPlayer()->Transform()->SetRelativeScale(Vec3(1, 1, 1));
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
}

void CKirbyAbility_Normal::AttackCharge2Exit()
{
}