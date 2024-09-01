#include "pch.h"
#include "CKirbyAbility_Sword.h"
#include "CKirbyMoveController.h"
#include "CState.h"
#include "CDestroyParticleScript.h"

CKirbyAbility_Sword::CKirbyAbility_Sword()
    : m_KirbySwordSlashPref(nullptr)
    , m_KirbySwordTwinkleParticlePref(nullptr)
    , m_KirbySwordFireParticlePref(nullptr)
    , m_BigWeaponScale(Vec3(5.f, 5.f, 5.f))
    , m_PrevSpeed(0.f)
    , m_PrevRotSpeed(0.f)
    , m_PrevGravity(0.f)
    , m_AccTime(0.f)
    , m_bFrmEnter(true)
{
    m_Hat = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordHat.pref", L"prefab\\KirbySwordHat.pref");
    m_Weapon = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordWeapon.pref", L"prefab\\KirbySwordWeapon.pref");
    m_KirbySwordSlashPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordSlash.pref");
    m_KirbySwordTwinkleParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordTwinkleParticle.pref");
    m_KirbySwordFireParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordFireParticle.pref");
    m_ComboSuccessTime = 0.5f;
    m_Charge1Time = 1.f;
    m_Charge2Time = 1.f;
}

CKirbyAbility_Sword::CKirbyAbility_Sword(const CKirbyAbility_Sword& _Origin)
    : CKirbyAbility(_Origin)
    , m_KirbySwordSlashPref(nullptr)
    , m_KirbySwordTwinkleParticlePref(nullptr)
    , m_KirbySwordFireParticlePref(nullptr)
    , m_BigWeaponScale(Vec3(5.f, 5.f, 5.f))
    , m_PrevSpeed(0.f)
    , m_PrevRotSpeed(0.f)
    , m_PrevGravity(0.f)
    , m_AccTime(0.f)
    , m_bFrmEnter(true)
{
    m_Hat = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordHat.pref", L"prefab\\KirbySwordHat.pref");
    m_Weapon = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordWeapon.pref", L"prefab\\KirbySwordWeapon.pref");
    m_KirbySwordSlashPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordSlash.pref");
    m_KirbySwordTwinkleParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordTwinkleParticle.pref");
    m_KirbySwordFireParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordFireParticle.pref");
    m_ComboSuccessTime = 0.5f;
    m_Charge1Time = 1.f;
    m_Charge2Time = 1.f;
}

CKirbyAbility_Sword::~CKirbyAbility_Sword()
{
}

// ===============
// Idle
// ===============

void CKirbyAbility_Sword::IdleEnter()
{
    CState* PrevState = PLAYERFSM->GetPrevState();

    wstring prev = L"";

    if (PrevState != nullptr)
    {
        prev = PLAYERFSM->GetPrevState()->GetName();
    }

    if (prev == L"ATTACK" || prev == L"ATTACK_COMBO1" || prev == L"ATTACK_COMBO2")
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Wait"), true, false, 2.5f, 0.3f);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Wait"));
    }
}

// ===============
// Attack
// ===============
// 칼휘두르기 (Lv0)

#include "CMomentaryObjScript.h"
void CKirbyAbility_Sword::Attack()
{
    if (CHECK_ANIMFRM(PLAYER, 7) && m_bFrmEnter)
    {
        m_bFrmEnter = false;
        SpawnSwordSlash(Vec3(30.f, 30.f, 30.f));
    }
}

void CKirbyAbility_Sword::AttackEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Attack1"), false, false, 1.5);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);

    m_bFrmEnter = true;

    //@Effect Effect 재생
}

void CKirbyAbility_Sword::AttackExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetComboLevel(1);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
}

// ===============
// Attack Combo 1
// ===============
// 칼휘두르기 (Lv1)

void CKirbyAbility_Sword::AttackCombo1()
{
    if (CHECK_ANIMFRM(PLAYER, 9) && m_bFrmEnter)
    {
        m_bFrmEnter = false;
        SpawnSwordSlash(Vec3(50.f, 30.f, 30.f));
    }
}

void CKirbyAbility_Sword::AttackCombo1Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Attack2"), false, false, 1.5f);
    //@Effect 재생

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);

    m_bFrmEnter = true;
}

void CKirbyAbility_Sword::AttackCombo1Exit()
{
    PLAYERFSM->SetComboLevel(2);

    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
}

// ===============
// Attack Combo 2
// ===============
// 칼휘두르기 (Lv2)

void CKirbyAbility_Sword::AttackCombo2()
{
    if (CHECK_ANIMFRM(PLAYER, 17) && m_bFrmEnter)
    {
        m_bFrmEnter = false;
        SpawnSwordSlash(Vec3(50.f, 30.f, 30.f));
    }
}

void CKirbyAbility_Sword::AttackCombo2Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Attack3"), false, false, 1.5f);

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    //@Effect Effect 재생
    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);

    m_bFrmEnter = true;
}

void CKirbyAbility_Sword::AttackCombo2Exit()
{
    PLAYERFSM->SetComboLevel(-1);

    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
}

// ===============
// Charge Attack 1
// ===============
// 스핀공격 (Lv1)

// (charge attack)
void CKirbyAbility_Sword::AttackCharge1()
{
    //@TODO Decal
}

void CKirbyAbility_Sword::AttackCharge1Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SpinSlash"), false, false, 1.5f);
    //@Effect 스핀이펙트

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
}

void CKirbyAbility_Sword::AttackCharge1Exit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
}

// Start (charge)
void CKirbyAbility_Sword::AttackCharge1Start()
{
}

void CKirbyAbility_Sword::AttackCharge1StartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SpinSlashCharge"), false, false, 1.5f);
    //@Effect 충전완료

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAbility_Sword::AttackCharge1StartExit()
{
    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// End (charge attack end)
void CKirbyAbility_Sword::AttackCharge1End()
{
}

void CKirbyAbility_Sword::AttackCharge1EndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SpinSlashEnd"), false, false, 1.5f);
    //@Effect 터지는 흰색 파티클 입자들

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetFriction(5.f);
    PLAYERCTRL->SetFrictionMode(true);

    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
}

void CKirbyAbility_Sword::AttackCharge1EndExit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetFrictionMode(false);

    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
}

// ===============
// Charge Attack 2
// ===============
// 스핀공격-차지 (Lv2)

// (charge)
void CKirbyAbility_Sword::AttackCharge2()
{
}

void CKirbyAbility_Sword::AttackCharge2Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SuperSpinSlashCharge"), true, false, 1.5f);

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAbility_Sword::AttackCharge2Exit()
{
    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// start (charge start)
void CKirbyAbility_Sword::AttackCharge2Start()
{
}

void CKirbyAbility_Sword::AttackCharge2StartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SuperSpinSlashChargeStart"), false, false, 1.5f);

    //@Effect 충전완료
    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAbility_Sword::AttackCharge2StartExit()
{
    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// ===============
// Charge Attack 2
// ===============
// 스핀공격-공격 (Lv2)
// State 따로 파서 구현

// ===============
// Charge Attack 3
// ===============
// 대검공격 (Lv3)

// (charge)
void CKirbyAbility_Sword::AttackCharge3()
{
    if (CHECK_ANIMFRM(PLAYER, 23) && !PLAYER->Animator()->IsReverse())
    {
        if (m_bFrmEnter)
        {
            PLAYER->Animator()->SetPlaySpeed(1.f);
            m_bFrmEnter = false;
        }

        PLAYER->Animator()->SetReverse(true);
    }
    else if (!PLAYER->Animator()->IsChainging() && PLAYER->Animator()->GetClipFrameIndex() < 21 && PLAYER->Animator()->IsReverse())
    {
        PLAYER->Animator()->SetReverse(false);
    }
}

void CKirbyAbility_Sword::AttackCharge3Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("GigantChargeAttack"), false, false, 1.5f);

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    m_PrevRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->SetRotSpeed(5.f);
    PLAYERCTRL->LockJump();

    PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(m_BigWeaponScale);

    m_bFrmEnter = true;
}

void CKirbyAbility_Sword::AttackCharge3Exit()
{
    PLAYER->Animator()->SetReverse(false);
    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->SetRotSpeed(m_PrevRotSpeed);
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(m_PrevWeaponScale);
}

// Start (charge start)
void CKirbyAbility_Sword::AttackCharge3Start()
{
    m_AccTime += DT;

    float t = max(m_AccTime / PLAYER->Animator()->GetClipPlayTime(), 1.f);
    Vec3 NewScale = m_PrevWeaponScale + (m_BigWeaponScale - m_PrevWeaponScale) * t;
    PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(NewScale);
}

void CKirbyAbility_Sword::AttackCharge3StartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("GigantChargeAttackStart"), false, false, 1.5f);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);
    //@Effect 충전완료

    m_AccTime = 0.f;
    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    m_PrevRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->SetRotSpeed(5.f);
    PLAYERCTRL->LockJump();

    m_PrevWeaponScale = PLAYERFSM->GetCurWeapon()->Transform()->GetLocalScale();
}

void CKirbyAbility_Sword::AttackCharge3StartExit()
{
    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->SetRotSpeed(m_PrevRotSpeed);
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(m_PrevWeaponScale);
}

// End (attack)
void CKirbyAbility_Sword::AttackCharge3End()
{
    float SizeChangeFrm = 50.f;

    if (CHECK_ANIMFRM(PLAYER, (int)SizeChangeFrm))
    {
        float t = 1.f - max((PLAYER->Animator()->GetClipFrameIndex() - SizeChangeFrm) / (72.f - SizeChangeFrm), 0.f);
        Vec3 NewScale = m_PrevWeaponScale + (m_BigWeaponScale - m_PrevWeaponScale) * t;
        PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(NewScale);
    }
}

void CKirbyAbility_Sword::AttackCharge3EndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("GigantChargeAttack"), false, false, 1.5f, 0.f);
    PLAYER->Animator()->SetClipFrameIndex(25);
    //@Effect 번개

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(m_BigWeaponScale);
}

void CKirbyAbility_Sword::AttackCharge3EndExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
    PLAYERFSM->GetCurWeapon()->Transform()->SetLocalScale(m_PrevWeaponScale);
}

// ===============
// Jump
// ===============
// 점프
void CKirbyAbility_Sword::JumpFallEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("JumpFall"), false, false, 2.5f, 0.3f);
}

// ===============
// Jump Attack
// ===============
// 점프 스핀공격

void CKirbyAbility_Sword::JumpAttack()
{
    if (m_bFrmEnter && CHECK_ANIMFRM(PLAYER, 10))
    {
        if (PLAYERFSM->GetSlideComboLevel())
        {
            PLAYERCTRL->SetGravity(-80.f);
        }
        else
        {
            PLAYERCTRL->SetGravity(m_PrevGravity);
        }
        m_bFrmEnter = false;
    }
}

void CKirbyAbility_Sword::JumpAttackEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SwordSpin"), false, false, 2.f);
    //@Effect 스핀 이펙트

    m_PrevGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);
    PLAYERCTRL->ClearVelocityY();
    PLAYERCTRL->LockDirection();

    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    m_bFrmEnter = true;
    SpawnSwordSlash(Vec3(50.f, 30.f, 30.f), true);
}

void CKirbyAbility_Sword::JumpAttackExit()
{
    PLAYERCTRL->SetGravity(m_PrevGravity);
    PLAYERCTRL->UnlockDirection();

    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
}

// Start
void CKirbyAbility_Sword::JumpAttackStart()
{
}

void CKirbyAbility_Sword::JumpAttackStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SwordSpinStart"), false, false, 2.f);
    //@Effect 칼 끝 따라가는 이펙트

    PLAYERCTRL->LockDirection();

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    PLAYERFSM->SetInvincible(true);
}

void CKirbyAbility_Sword::JumpAttackStartExit()
{
    PLAYERCTRL->UnlockDirection();

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
    PLAYERFSM->SetInvincible(false);
}

// ===============
// Landing
// ===============

void CKirbyAbility_Sword::LandingEnter()
{
    if (PLAYERFSM->GetLastJump() == LastJumpType::LOW)
    {
        PLAYER->Animator()->Play(ANIMPREFIX("LandingSmall"), false);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Landing"), false);
    }

    if (PLAYERFSM->GetSlideComboLevel())
    {
        PLAYERFSM->SetKnockBackDir(-PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT));
        PLAYERFSM->SetInvincible(true);

        //@Effect 내려찍는 충격효과, 나비
    }

    PLAYERCTRL->LockJump();
}

void CKirbyAbility_Sword::LandingExit()
{
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetInvincible(false);
}

// ===============
// Guard
// ===============
void CKirbyAbility_Sword::GuardEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("GuardMask"), true, false, 1.5f, 0.2);
}

// ===============
// Guard Run
// ===============
// 순간이동

void CKirbyAbility_Sword::GuardRun()
{
}

void CKirbyAbility_Sword::GuardRunEnter()
{
    PLAYER->MeshRender()->SetEnabled(false);
    PLAYERFSM->GetCurHat()->MeshRender()->SetEnabled(false);
    PLAYERFSM->GetCurWeapon()->MeshRender()->SetEnabled(false);
    //@Effect 궤적 파티클

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(15.f);
    PLAYERCTRL->LockJump();

    PLAYERFSM->SetInvincible(true);
}

void CKirbyAbility_Sword::GuardRunExit()
{
    PLAYER->MeshRender()->SetEnabled(true);
    PLAYERFSM->GetCurHat()->MeshRender()->SetEnabled(true);
    PLAYERFSM->GetCurWeapon()->MeshRender()->SetEnabled(true);

    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockJump();
    Vec3 vel = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetVelocity(vel);

    PLAYERFSM->SetInvincible(false);
}

// ===============
// Slide
// ===============
// 슬라이드 찌르기

void CKirbyAbility_Sword::Slide()
{
}

void CKirbyAbility_Sword::SlideEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SwordSlide"));

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    PLAYERFSM->SetInvincible(true);
}

void CKirbyAbility_Sword::SlideExit()
{
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
    PLAYERFSM->SetInvincible(false);
}

// Start
void CKirbyAbility_Sword::SlideStart()
{
}

void CKirbyAbility_Sword::SlideStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SwordSlideStart"), false);

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    PLAYERFSM->SetInvincible(true);
}

void CKirbyAbility_Sword::SlideStartExit()
{
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
    PLAYERFSM->SetInvincible(false);
}

// End
void CKirbyAbility_Sword::SlideEnd()
{
}

void CKirbyAbility_Sword::SlideEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SwordSlideEnd"), false);

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    PLAYERFSM->SetInvincible(true);
}

void CKirbyAbility_Sword::SlideEndExit()
{
    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
    PLAYERFSM->SetInvincible(false);
}

// ===============
// Slide Attack
// ===============
// 슬라이드 콤보공격

void CKirbyAbility_Sword::SlideAttack()
{
    if (PLAYER->Animator()->IsFinish() && m_bFrmEnter)
    {
        if (PLAYERCTRL->GetVelocity().y > 0.f)
        {
            PLAYERCTRL->ClearVelocityY();
        }

        m_bFrmEnter = false;
    }
}

void CKirbyAbility_Sword::SlideAttackEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("UpwardSlash"), false, false, 2.f);
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);
    //@Effect 칼끝 궤적선

    PLAYERCTRL->AddVelocity(Vec3(0, 30, 0));
    m_PrevGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(-100.f);

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(true);
    PLAYERFSM->SetInvincible(true);

    m_bFrmEnter = true;
}

void CKirbyAbility_Sword::SlideAttackExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->SetGravity(m_PrevGravity);
    if (PLAYERCTRL->GetVelocity().y > 0.f)
    {
        PLAYERCTRL->ClearVelocityY();
    }

    PLAYERFSM->GetCurWeapon()->BoxCollider()->SetEnabled(false);
    PLAYERFSM->SetInvincible(false);
}

// ===============
// Change Ability
// ===============

void CKirbyAbility_Sword::ChangeAbility()
{
}

void CKirbyAbility_Sword::ChangeAbilityEnter()
{
    // create hat
    CGameObject* pInstObj = m_Hat->Instantiate();
    PLAYERFSM->SetCurHat(pInstObj);
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    // particle
    pInstObj = m_KirbySwordTwinkleParticlePref->Instantiate();
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    // create sword
    pInstObj = m_Weapon->Instantiate();
    pInstObj->BoxCollider()->SetEnabled(false);
    PLAYERFSM->SetCurWeapon(pInstObj);
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Weapon");

    // Sword Fire Particle
    CGameObject* pFireParticle = m_KirbySwordFireParticlePref->Instantiate();
    pFireParticle->ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, false);
    GamePlayStatic::AddChildObject(pInstObj, pFireParticle, L"WeaponEnd");
}

void CKirbyAbility_Sword::ChangeAbilityExit()
{
}

// ===============
// Drop Ability
// ===============

void CKirbyAbility_Sword::DropAbilityEnter()
{
    CGameObject* pTwinkleParticle = PLAYER->GetChildObject(L"KirbySwordTwinkleParticle");
    if (nullptr != pTwinkleParticle)
    {
        GamePlayStatic::DestroyGameObject(pTwinkleParticle);
    }
}

// ===============
// Custom Func
// ===============

void CKirbyAbility_Sword::SpawnSwordSlash(Vec3 _SlashScale, bool _bVertical)
{
    if (m_KirbySwordSlashPref == nullptr || PLAYERUNIT->GetCurInfo().HP < PLAYERUNIT->GetCurInfo().MAXHP)
        return;

    CGameObject* SwordSlash = m_KirbySwordSlashPref->Instantiate();
    Vec3 Dir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 Pos = PLAYER->Transform()->GetWorldPos() + Dir * 10.f;

    SwordSlash->Transform()->SetWorldScale(_SlashScale);
    float ImpulsePower = (_SlashScale.x + _SlashScale.y + _SlashScale.z) / 3.f;
    ImpulsePower *= 1.2f;

    if (_bVertical)
    {
        SwordSlash->Transform()->SetWorldPos(Pos);
        SwordSlash->Transform()->SetDirection(Dir, PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT));
    }
    else
    {
        SwordSlash->Transform()->SetWorldPos(Pos + Vec3(0.f, 20.f, 0.f));
        SwordSlash->Transform()->SetDirection(Dir);
    }

    CMomentaryObjScript* Script = SwordSlash->GetScript<CMomentaryObjScript>();
    Script->SetInitVelocity(Dir * ImpulsePower);
    Script->SetPlayTime(1.5f);

    SwordSlash->SetName(L"KirbyAttack_SwordSlash");
    GamePlayStatic::SpawnGameObject(SwordSlash, LAYER_PLAYERATK_TRIGGER);
}
