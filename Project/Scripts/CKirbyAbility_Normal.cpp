#include "pch.h"
#include "CKirbyAbility_Normal.h"
#include "CKirbyBulletScript.h"
#include "CKirbyMoveController.h"
#include "CKirbyVacuumCollider.h"

CKirbyAbility_Normal::CKirbyAbility_Normal()
    : m_bFrmEnter(true)
    , m_SavedSpeed(0.f)
{
    m_Charge1Time = 2.f;
}

CKirbyAbility_Normal::~CKirbyAbility_Normal()
{
}

// ===============
// Attack
// ===============
// ¸Ó±ÝÀº ¹°Ã¼ ¹ñ±â
// - StuffedÀÏ ¶§¸¸ µé¾î¿È

void CKirbyAbility_Normal::Attack()
{
    if (PLAYER->Animator()->GetClipFrameIndex() == 3 && m_bFrmEnter)
    {
        CPlayerMgr::ClearBodyMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));
        m_bFrmEnter = false;

        // fire bullet
        Ptr<CPrefab> BulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBullet.pref", L"prefab\\KirbyBullet.pref");
        if (nullptr != BulletPref)
        {
            CGameObject* BulletInst = BulletPref->Instantiate();
            Vec3 InitPos = PLAYER->Transform()->GetWorldPos();
            InitPos += PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 3.f * PLAYER->Transform()->GetLocalScale();
            InitPos += PLAYER->Transform()->GetWorldDir(DIR_TYPE::UP) * 2.f * PLAYER->Transform()->GetLocalScale();

            if (PLAYERFSM->GetStuffedCopyObj()->MeshRender())
            {
                BulletInst->AddComponent(PLAYERFSM->GetStuffedCopyObj()->MeshRender()->Clone());
            }
            BulletInst->Transform()->SetLocalPos(InitPos);
            GamePlayStatic::SpawnGameObject(BulletInst, LAYER_PLAYERATK);

            CKirbyBulletScript* bulletScript = BulletInst->GetScript<CKirbyBulletScript>();
            if (nullptr != bulletScript)
            {
                bulletScript->SetInitVelocity(PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 120.f);
                Vec3 InitDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
                InitDir.y = 0.f;
                bulletScript->SetInitVelocity(InitDir * 120.f);
            }
        }
    }
}

void CKirbyAbility_Normal::AttackEnter()
{
    if (PLAYER->CharacterController()->IsGrounded())
    {
        PLAYER->Animator()->Play(ANIMPREFIX(L"Spit"), false);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX(L"SpitAir"), false);
    }

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();

    m_bFrmEnter = true;
}

void CKirbyAbility_Normal::AttackExit()
{
    PLAYERFSM->ClearStuff();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
}

// ===============
// Charge Attack 1
// ===============
// ÈíÀÔÇÏ±â1

void CKirbyAbility_Normal::AttackCharge1()
{
}

void CKirbyAbility_Normal::AttackCharge1Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"Vacuum"), true, false, 2.f);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(4.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyAbility_Normal::AttackCharge1Exit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}

// start
void CKirbyAbility_Normal::AttackCharge1Start()
{
}

void CKirbyAbility_Normal::AttackCharge1StartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"VacuumStart2"), false);

    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);
}

void CKirbyAbility_Normal::AttackCharge1StartExit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}

// end
void CKirbyAbility_Normal::AttackCharge1End()
{
}

void CKirbyAbility_Normal::AttackCharge1EndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"VacuumEnd"), false);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);
}

void CKirbyAbility_Normal::AttackCharge1EndExit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}

// Run
void CKirbyAbility_Normal::AttackCharge1Run()
{
}

void CKirbyAbility_Normal::AttackCharge1RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"VacuumWalk"), true, false, 2.f);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyAbility_Normal::AttackCharge1RunExit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}

// ===============
// Charge Attack 2
// ===============
// ÈíÀÔÇÏ±â2
void CKirbyAbility_Normal::AttackCharge2()
{
}

void CKirbyAbility_Normal::AttackCharge2Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"SuperInhale"), true, false, 2.f);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyAbility_Normal::AttackCharge2Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}

// start
void CKirbyAbility_Normal::AttackCharge2Start()
{
}

void CKirbyAbility_Normal::AttackCharge2StartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"SuperInhaleStart"), false);
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyAbility_Normal::AttackCharge2StartExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}

// run
void CKirbyAbility_Normal::AttackCharge2Run()
{
}

void CKirbyAbility_Normal::AttackCharge2RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"SuperInhaleWalk"), true, false, 2.f);

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    PLAYERFSM->GetVacuumCol()->EnableCollider(true);
}

void CKirbyAbility_Normal::AttackCharge2RunExit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->GetVacuumCol()->EnableCollider(false);
}

// ===============
// Change Ability
// ===============
void CKirbyAbility_Normal::ChangeAbility()
{
}

void CKirbyAbility_Normal::ChangeAbilityEnter()
{
    PLAYERFSM->SetCurHat(nullptr);
    PLAYERFSM->SetCurWeapon(nullptr);
}

void CKirbyAbility_Normal::ChangeAbilityExit()
{
}
