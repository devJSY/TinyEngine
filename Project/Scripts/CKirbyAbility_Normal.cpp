#include "pch.h"
#include "CKirbyAbility_Normal.h"
#include "CKirbyBulletScript.h"

#define BULLET_SPEED 5.f

CKirbyAbility_Normal::CKirbyAbility_Normal()
    : m_FrmEnter(true)
    , m_Charge1Time(3.f)
{
}

CKirbyAbility_Normal::~CKirbyAbility_Normal()
{
}

void CKirbyAbility_Normal::Idle()
{
    if (KEY_TAP(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if ((bool)PLAYERFSM->IsCharge() && (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)))
    {
        ChangeState(L"ATTACK_CHARGE1_END");
    }
    else if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
    {
        ChangeState(L"RUN_START");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"JUMP_START");
    }
}

void CKirbyAbility_Normal::IdleEnter()
{
    if (PLAYERFSM->IsStuffed())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"StuffedWait"));
    }
    else if (PLAYERFSM->IsCharge() == ChargeType::LV1)
    {
        ChangeState(L"ATTACK_CHARGE1");
    }
    else if (PLAYERFSM->IsCharge() == ChargeType::LV2)
    {
        ChangeState(L"ATTACK_CHARGE2");
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Wait"));
    }
}

void CKirbyAbility_Normal::IdleExit()
{
}

// ===============
// RUN
// ===============
void CKirbyAbility_Normal::Run()
{
    if ((bool)PLAYERFSM->IsCharge() && (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)))
    {
        ChangeState(L"ATTACK_CHARGE1_END");
    }
}

void CKirbyAbility_Normal::RunEnter()
{
    if ((bool)PLAYERFSM->IsCharge())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"VacuumWalk"));
    }
    else if (PLAYERFSM->IsStuffed())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"StuffedRun"));
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Run"));
    }
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
    if ((bool)PLAYERFSM->IsCharge() || PLAYERFSM->IsStuffed())
        ChangeState(L"RUN");
    else
        PLAYER->Animator()->Play(KIRBYANIM(L"RunStart"), false);
}

void CKirbyAbility_Normal::RunStartExit()
{
}

// ===============
// Attack
// ===============
// 머금은 물체 뱉기

void CKirbyAbility_Normal::Attack()
{
    // Attack: Stuffed 커비가 별을 뱉는 상태
    if (!PLAYERFSM->IsStuffed())
        return;

    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
        return;
    }

    if (GET_CURCLIP_FRM == 3 && m_FrmEnter)
    {
        // @TODO Material 이름으로 받아오기
        PLAYER->GetRenderComponent()->SetMaterial(nullptr, 1);
        m_FrmEnter = false;
    }
}

void CKirbyAbility_Normal::AttackEnter()
{
    // ===================
    // case: Start Vacuum
    // ===================
    if (!PLAYERFSM->IsStuffed())
    {
        ChangeState(L"ATTACK_CHARGE1_START");
        return;
    }

    // ===================
    // case: Shoot Bullet
    // ===================
    // @TODO 점프상태일 때 (Animation: SpitAir)
    PLAYER->Animator()->Play(KIRBYANIM(L"Spit"), false);
    m_FrmEnter = true;

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

void CKirbyAbility_Normal::AttackExit()
{
    PLAYERFSM->SetStuffed(false);
    m_FrmEnter = true;
}

// ===============
// Charge Attack 1
// ===============
// 흡입하기1

void CKirbyAbility_Normal::AttackCharge1()
{
    if (PLAYERFSM->GetChargeAccTime() >= m_Charge1Time)
    {
        ChangeState(L"ATTACK_CHARGE2");
    }
    if (KEY_TAP(KEY::ENTER))
    {
        // @TODO 테스트용코드
        ChangeState(L"STUFFED");
    }

    if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        ChangeState(L"ATTACK_CHARGE1_END");
    }
    else if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
    {
        ChangeState(L"RUN_START");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"JUMP_START");
    }
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
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"ATTACK_CHARGE1");
    }
}

void CKirbyAbility_Normal::AttackCharge1StartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumStart2"), false);
    // @TODO Material 이름으로 받아오기 & material key값 변경
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 6);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 0);
    PLAYERFSM->SetCharge(ChargeType::LV1);
    // @TODO 속도조절
}

void CKirbyAbility_Normal::AttackCharge1StartExit()
{
}

// end
void CKirbyAbility_Normal::AttackCharge1End()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyAbility_Normal::AttackCharge1EndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumEnd"), false);
    // @TODO Material 이름으로 받아오기 & material key값 변경
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 0);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 6);
    PLAYERFSM->SetCharge(ChargeType::NONE);
    // @TODO 속도조절
}

void CKirbyAbility_Normal::AttackCharge1EndExit()
{
    PLAYERFSM->ClearChargeAccTime();
}

// ===============
// Charge Attack 2
// ===============
// 흡입하기2
void CKirbyAbility_Normal::AttackCharge2()
{
    if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        ChangeState(L"ATTACK_CHARGE1_END");
    }
    else if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
    {
        ChangeState(L"RUN_START");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"JUMP_START");
    }
}

void CKirbyAbility_Normal::AttackCharge2Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"VacuumHustleLv2"));
    // @TODO Material 이름으로 받아오기 & material key값 변경
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 0);
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 6);
    PLAYERFSM->SetCharge(ChargeType::LV2);
    // @TODO 속도조절
}

void CKirbyAbility_Normal::AttackCharge2Exit()
{
}