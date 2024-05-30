#include "pch.h"
#include "CPlayerScript.h"
#include <Engine\\CPhysics2DMgr.h>
#include <Engine\\CLevelMgr.h>

#include <Engine\\CLevel.h>
#include <Engine\\CAnim.h>
#include "CPlayerCameraScript.h"
#include "CPlayerHitBoxScript.h"
#include "CBridgeScript.h"
#include "CPlayerDamageLifeBarScript.h"

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_State(PLAYER_STATE::Idle)
    , m_Dir(DIRECTION_TYPE::RIGHT)
    , m_Scythe(PLAYER_WEAPON_SCYTHE::TheScythe)
    , m_Cloak(PLAYER_WEAPON_CLOAK::Slaymore)
    , m_Spell(PLAYER_WEAPON_SPELL::WizzalchBarrage)
    , m_MaxLife(65)
    , m_MaxMana(50)
    , m_CurLife(m_MaxLife)
    , m_CurMana(m_MaxMana)
    , m_ATK(10)
    , m_Speed(10.f)
    , m_JumpImpulse(1.f)
    , m_JumpForce(1.f)
    , m_DashImpulse(1.f)
    , m_AttackImpulse(1.f)
    , m_RaycastDist(100.f)
    , m_bOnGround(false)
    , m_DashPassedTime(0.f)
    , m_DashCoolTime(1.f)
    , m_RigidGravityScale(0.f)
    , m_bJumpAttackActive(true)
    , m_AttackCount(0)
    , m_ManaDelay(0.f)
    , m_pShockWavePref(nullptr)
    , m_pLightningSpawner02Pref(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxLife, "Max Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxMana, "Max Mana");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurLife, "Current Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurMana, "Current Mana");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpImpulse, "Jump Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpForce, "Jump Force");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_DashImpulse, "Dash Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackImpulse, "Attack Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RaycastDist, "Raycast Distance");
}

CPlayerScript::CPlayerScript(const CPlayerScript& origin)
    : CScript(origin)
    , m_State(origin.m_State)
    , m_Dir(origin.m_Dir)
    , m_Scythe(origin.m_Scythe)
    , m_Cloak(origin.m_Cloak)
    , m_Spell(origin.m_Spell)
    , m_MaxLife(origin.m_MaxLife)
    , m_MaxMana(origin.m_MaxMana)
    , m_CurLife(origin.m_CurLife)
    , m_CurMana(origin.m_CurMana)
    , m_ATK(origin.m_ATK)
    , m_Speed(origin.m_Speed)
    , m_JumpImpulse(origin.m_JumpImpulse)
    , m_JumpForce(origin.m_JumpForce)
    , m_DashImpulse(origin.m_DashImpulse)
    , m_AttackImpulse(origin.m_AttackImpulse)
    , m_RaycastDist(origin.m_RaycastDist)
    , m_bOnGround(origin.m_bOnGround)
    , m_DashPassedTime(origin.m_DashPassedTime)
    , m_DashCoolTime(origin.m_DashCoolTime)
    , m_RigidGravityScale(origin.m_RigidGravityScale)
    , m_bJumpAttackActive(origin.m_bJumpAttackActive)
    , m_AttackCount(origin.m_AttackCount)
    , m_ManaDelay(0.f)
    , m_pShockWavePref(origin.m_pShockWavePref)
    , m_pLightningSpawner02Pref(origin.m_pLightningSpawner02Pref)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxLife, "Max Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxMana, "Max Mana");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurLife, "Current Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurMana, "Current Mana");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpImpulse, "Jump Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpForce, "Jump Force");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_DashImpulse, "Dash Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackImpulse, "Attack Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RaycastDist, "Raycast Distance");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DGlowMtrl"));

        Transform()->SetRelativeScale(Vec3(1024.f, 1024.f, 1.f));
    }

    MeshRender()->CreateDynamicMaterial();

    // �ִϸ��̼� �ε�
    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_IdleToRun.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_IdleUturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Falling.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Landing.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Start.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Run.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_RunToIdle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_RunUturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Dash.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Hit01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Hit02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Fight_To_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\Elevator_In.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\Elevator_End.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_Out.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Ritual_End_Boss.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Spawn_Lobby.anim");

        // TheScythe
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove04.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboStand1.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboStand2.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboStand3.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboStand4.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Special2.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Crush.anim");

        // HeavySword
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboHeavySword_1.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboHeavySword_2.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboHeavySword_3.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboHeavySword_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboHeavySword_Up.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboHeavySword_UpDown.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();

    ChangeWeapon(m_Scythe);
    ChangeWeapon(m_Cloak);
    ChangeWeapon(m_Spell);

    // ������ �ε�
    m_pShockWavePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FX_Shockwave01_Atlas.pref", L"prefab\\FX_Shockwave01_Atlas.pref");
    m_pLightningSpawner02Pref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\LightningSpawner02.pref", L"prefab\\LightningSpawner02.pref");
}

void CPlayerScript::tick()
{
    // FSM
    switch (m_State)
    {
    case PLAYER_STATE::Idle:
        Idle();
        break;
    case PLAYER_STATE::IdleToRun:
        IdleToRun();
        break;
    case PLAYER_STATE::IdleUturn:
        IdleUturn();
        break;
    case PLAYER_STATE::Jump_Falling:
        Jump_Falling();
        break;
    case PLAYER_STATE::Jump_Start:
        Jump_Start();
        break;
    case PLAYER_STATE::Jump_Landing:
        Jump_Landing();
        break;
    case PLAYER_STATE::Run:
        Run();
        break;
    case PLAYER_STATE::RunUturn:
        RunUturn();
        break;
    case PLAYER_STATE::RunToIdle:
        RunToIdle();
        break;
    case PLAYER_STATE::Dash:
        Dash();
        break;
    case PLAYER_STATE::Hit01:
        Hit01();
        break;
    case PLAYER_STATE::Hit02:
        Hit02();
        break;
    case PLAYER_STATE::Fight_To_Idle:
        Fight_To_Idle();
        break;
    case PLAYER_STATE::Elevator_In:
        Elevator_In();
        break;
    case PLAYER_STATE::Elevator_End:
        Elevator_End();
        break;
    case PLAYER_STATE::Elevator_Out:
        Elevator_Out();
        break;
    case PLAYER_STATE::Cinematic:
        Cinematic();
        break;
    case PLAYER_STATE::EndBoss:
        EndBoss();
        break;
    case PLAYER_STATE::SpawnLobby:
        SpawnLobby();
        break;
    case PLAYER_STATE::TheScythe_Attack:
        TheScythe_Attack();
        break;
    case PLAYER_STATE::TheScythe_AerialAttack:
        TheScythe_AerialAttack();
        break;
    case PLAYER_STATE::TheScythe_JumpAttack:
        TheScythe_JumpAttack();
        break;
    case PLAYER_STATE::TheScythe_DownAttack:
        TheScythe_DownAttack();
        break;
    case PLAYER_STATE::Slaymore_UpDown:
        Slaymore_UpDown();
        break;
    }

    // Mana ���
    m_ManaDelay += DT;
    if (m_ManaDelay > 1.f)
    {
        static float fTerm = 0.f;
        fTerm += DT;
        if (fTerm > 0.05f && m_CurMana < m_MaxMana)
        {
            m_CurMana++;
            fTerm = 0.f;
        }
    }

    // FSM�� ���� Bloom ���� ����
    if (PLAYER_STATE::Dash == m_State || (PLAYER_STATE::TheScythe_Attack <= m_State && PLAYER_STATE::Slaymore_UpDown >= m_State))
    {
        MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1);
        MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.5f);
        MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(0.48f, 0.40f, 0.78f, 1.f));
    }
    else
    {
        MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
    }

    m_DashPassedTime += DT;

    GroundCheck();

    // �׽�Ʈ �ڵ�
    if (KEY_TAP(KEY::Z))
    {
        m_ATK = 10;
    }
    else if (KEY_TAP(KEY::C))
    {
        m_ATK += 10;
    }
}

void CPlayerScript::ChangeState(PLAYER_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

bool CPlayerScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (m_State == PLAYER_STATE::Dash || m_State == PLAYER_STATE::TheScythe_DownAttack || m_State == PLAYER_STATE::TheScythe_JumpAttack ||
        m_State == PLAYER_STATE::Slaymore_UpDown)
        return false;

    m_CurLife -= _DamageAmount;

    // �ӽ� ü�� ����
    if (m_CurLife <= 0)
        m_CurLife = m_MaxLife;

    Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
    Force.Normalize();
    Force *= 100.f;
    Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

    if (_DamageAmount < 10)
        ChangeState(PLAYER_STATE::Hit01);
    else
        ChangeState(PLAYER_STATE::Hit02);

    // Damage LifeBar Update
    CGameObject* pDamageLifeBar = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectOfType<CPlayerDamageLifeBarScript>();
    if (nullptr != pDamageLifeBar)
    {
        pDamageLifeBar->GetScript<CPlayerDamageLifeBarScript>()->TakeHit(1.f);
    }

    return true;
}

void CPlayerScript::ChangeWeapon(PLAYER_WEAPON_SCYTHE _Scythe)
{
    m_Scythe = _Scythe;

    Ptr<CMaterial> pScytheMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\WeaponScytheMtrl.mtrl");

    switch (m_Scythe)
    {
    case PLAYER_WEAPON_SCYTHE::TheScythe:
        pScytheMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\UI\\PLACEHOLDER\\PLACEHOLDER_Scythe_Basic.png",
                                                                             L"Texture\\UI\\PLACEHOLDER\\PLACEHOLDER_Scythe_Basic.png"));
        break;
    case PLAYER_WEAPON_SCYTHE::NONE:
        pScytheMtrl->SetTexParam(TEX_0, nullptr);
        break;
    }
}

void CPlayerScript::ChangeWeapon(PLAYER_WEAPON_CLOAK _Cloak)
{
    m_Cloak = _Cloak;

    Ptr<CMaterial> pCloakMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\WeaponCloakMtrl.mtrl");
    switch (m_Cloak)
    {
    case PLAYER_WEAPON_CLOAK::Slaymore:
        pCloakMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\UI\\PLACEHOLDER\\PLACEHOLDER_Zweihander.png",
                                                                            L"Texture\\UI\\PLACEHOLDER\\PLACEHOLDER_Zweihander.png"));
        break;
    case PLAYER_WEAPON_CLOAK::NONE:
        pCloakMtrl->SetTexParam(TEX_0, nullptr);
        break;
    }
}

void CPlayerScript::ChangeWeapon(PLAYER_WEAPON_SPELL _Spell)
{
    m_Spell = _Spell;

    Ptr<CMaterial> pSpellMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\WeaponSpellMtrl.mtrl");
    switch (m_Spell)
    {
    case PLAYER_WEAPON_SPELL::WizzalchBarrage:
        pSpellMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\UI\\PLACEHOLDER\\PLACEHOLDER_TopDanger.png",
                                                                            L"Texture\\UI\\PLACEHOLDER\\PLACEHOLDER_TopDanger.png"));
        break;
    case PLAYER_WEAPON_SPELL::NONE:
        pSpellMtrl->SetTexParam(TEX_0, nullptr);
        break;
    }
}
void CPlayerScript::EnterState()
{
    switch (m_State)
    {
    case PLAYER_STATE::Idle: {
        Animator2D()->Play(L"LD_Idle");
        m_AttackCount = 0;
    }
    break;
    case PLAYER_STATE::IdleToRun: {
        Animator2D()->Play(L"LD_IdleToRun");
    }
    break;
    case PLAYER_STATE::IdleUturn: {
        Animator2D()->Play(L"LD_IdleUturn");
    }
    break;
    case PLAYER_STATE::Jump_Falling: {
        Animator2D()->Play(L"LD_Jump_Falling");
    }
    break;
    case PLAYER_STATE::Jump_Start: {
        Rigidbody2D()->AddForce(Vec2(0.f, m_JumpImpulse), ForceMode2D::Impulse);
        Animator2D()->Play(L"LD_Jump_Start", false);
        GamePlayStatic::Play2DSound(L"sound\\Player\\PC_Nav_Jump_Lgt_01.wav", 1, 0.5f);
        EnableParticle(true);
    }
    break;
    case PLAYER_STATE::Jump_Landing: {
        Animator2D()->Play(L"LD_Jump_Landing", false);
    }
    break;
    case PLAYER_STATE::Run: {
        Animator2D()->Play(L"LD_Run");
    }
    break;
    case PLAYER_STATE::RunUturn: {
        Animator2D()->Play(L"LD_RunUturn", false);
    }
    break;
    case PLAYER_STATE::RunToIdle: {
        Animator2D()->Play(L"LD_RunToIdle", false);
    }
    break;
    case PLAYER_STATE::Dash: {
        Animator2D()->Play(L"LD_Dash", false);
        GamePlayStatic::Play2DSound(L"sound\\Player\\PC_Nav_Dash_Lgt_01.wav", 1, 0.5f);
        m_RigidGravityScale = Rigidbody2D()->GetGravityScale();
        Rigidbody2D()->SetGravityScale(0.f);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));

        if (DIRECTION_TYPE::LEFT == m_Dir)
            Rigidbody2D()->AddForce(Vec2(-m_DashImpulse, 0.f), ForceMode2D::Impulse);
        else
            Rigidbody2D()->AddForce(Vec2(m_DashImpulse, 0.f), ForceMode2D::Impulse);

        m_DashPassedTime = 0.f;

        EnableParticle(true);

        // ShockWave Effect
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        int EffectIdx = pCurLevel->FindLayerIndexByName(L"Effect");
        if (-1 == EffectIdx)
            EffectIdx = 0;

        CGameObject* pShockWaveObj = m_pShockWavePref->Instantiate();
        pShockWaveObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
        if (m_Dir == DIRECTION_TYPE::LEFT)
            pShockWaveObj->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
        GamePlayStatic::SpawnGameObject(pShockWaveObj, EffectIdx);
    }
    break;
    case PLAYER_STATE::Hit01: {
        GamePlayStatic::Play2DSound(L"sound\\Player\\Onos_Player_Death_Hit_light_01.wav", 1, 0.5f);
        Animator2D()->Play(L"LD_Hit01", false);
    }
    break;
    case PLAYER_STATE::Hit02: {
        GamePlayStatic::Play2DSound(L"sound\\Player\\Onos_Player_Death_Hit_light_02.wav", 1, 0.5f);
        Animator2D()->Play(L"LD_Hit02", false);
    }
    break;
    case PLAYER_STATE::Fight_To_Idle: {
        Animator2D()->Play(L"LD_Fight_To_Idle", false);
    }
    break;
    case PLAYER_STATE::Elevator_In: {
        StopWalking();
        Animator2D()->Play(L"Elevator_In", false);
    }
    break;
    case PLAYER_STATE::Elevator_End: {
        Animator2D()->Play(L"Elevator_End", false);
    }
    break;
    case PLAYER_STATE::Elevator_Out: {
        Animator2D()->Play(L"LD_Elevator_Out", false);
    }
    break;
    case PLAYER_STATE::Cinematic: {
        StopWalking();
        Animator2D()->Play(L"LD_Idle");
    }
    break;
    case PLAYER_STATE::EndBoss: {
        StopWalking();
        Animator2D()->Play(L"LD_Ritual_End_Boss", false);
    }
    break;
    case PLAYER_STATE::SpawnLobby: {
        StopWalking();
        Animator2D()->Play(L"LD_Spawn_Lobby", false);
    }
    break;
    case PLAYER_STATE::TheScythe_Attack: {
        if (0 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove01", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\ComboAttack1.wav", 1, 0.5f);
            SetHitBox(true, L"HitBox1");
        }
        else if (1 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove02", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\ComboAttack2.wav", 1, 0.5f);
            SetHitBox(false, L"HitBox1");
            SetHitBox(true, L"HitBox2");
        }
        else if (2 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove03", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\ComboAttack3.wav", 1, 0.5f);
            SetHitBox(false, L"HitBox2");
            SetHitBox(true, L"HitBox1");
        }
        else if (3 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove04", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\ComboAttack4.wav", 1, 0.5f);
            SetHitBox(false, L"HitBox1");
            SetHitBox(true, L"HitBox3");
        }

        StopWalking();
        if (0 != m_AttackCount)
        {
            if (DIRECTION_TYPE::LEFT == m_Dir)
                Rigidbody2D()->AddForce(Vec2(-m_AttackImpulse, 0.f), ForceMode2D::Impulse);
            else
                Rigidbody2D()->AddForce(Vec2(m_AttackImpulse, 0.f), ForceMode2D::Impulse);
        }
    }
    break;
    case PLAYER_STATE::TheScythe_AerialAttack: {
        if (0 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboStand1", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\JumpComboAttack1.wav", 1, 0.5f);
            SetHitBox(true, L"HitBox3");
        }
        else if (1 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove01", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\JumpComboAttack2.wav", 1, 0.5f);
            SetHitBox(false, L"HitBox3");
            SetHitBox(true, L"HitBox1");
        }
        else if (2 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboStand2", false);
            GamePlayStatic::Play2DSound(L"sound\\Player\\JumpComboAttack3.wav", 1, 0.5f);
            SetHitBox(false, L"HitBox1");
            SetHitBox(true, L"HitBox3");
        }

        m_RigidGravityScale = Rigidbody2D()->GetGravityScale();
        Rigidbody2D()->SetGravityScale(0.f);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));

        if (0 != m_AttackCount)
        {
            if (DIRECTION_TYPE::LEFT == m_Dir)
                Rigidbody2D()->AddForce(Vec2(-m_AttackImpulse, 0.f), ForceMode2D::Impulse);
            else
                Rigidbody2D()->AddForce(Vec2(m_AttackImpulse, 0.f), ForceMode2D::Impulse);
        }
    }
    break;
    case PLAYER_STATE::TheScythe_JumpAttack: {
        Animator2D()->Play(L"LD_Special2", false);
        GamePlayStatic::Play2DSound(L"sound\\Player\\JumpAttack.wav", 1, 0.5f);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));
        Rigidbody2D()->AddForce(Vec2(0.f, m_JumpImpulse * 1.5f), ForceMode2D::Impulse);

        m_bJumpAttackActive = false;

        SetHitBox(true, L"HitBox2");
    }
    break;
    case PLAYER_STATE::TheScythe_DownAttack: {
        Animator2D()->Play(L"LD_Crush", false);
        GamePlayStatic::Play2DSound(L"sound\\Player\\DownAttack.wav", 1, 0.5f);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));
        Rigidbody2D()->AddForce(Vec2(0.f, -m_JumpImpulse * 3.f), ForceMode2D::Impulse);

        SetHitBox(true, L"HitBox2");
    }
    break;
    case PLAYER_STATE::Slaymore_UpDown: {
        m_ATK = 85;
        m_RigidGravityScale = Rigidbody2D()->GetGravityScale();
        Rigidbody2D()->SetGravityScale(0.f);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));

        if (m_bOnGround)
            Rigidbody2D()->AddForce(Vec2(0.f, m_JumpImpulse * 0.8f), ForceMode2D::Impulse);

        Animator2D()->Play(L"LD_ComboHeavySword_UpDown", false);
        GamePlayStatic::Play2DSound(L"sound\\Player\\PC_Atk_Y_SwordBig_Up_01.wav", 1, 0.5f);

        SetHitBox(true, L"HitBox3");
    }
    break;
    }
}

void CPlayerScript::ExitState()
{
    switch (m_State)
    {
    case PLAYER_STATE::Idle: {
    }
    break;
    case PLAYER_STATE::IdleToRun: {
    }
    break;
    case PLAYER_STATE::IdleUturn: {
        RotateTransform();
    }
    break;
    case PLAYER_STATE::Jump_Falling: {
    }
    break;
    case PLAYER_STATE::Jump_Start: {
        EnableParticle(false);
    }
    break;
    case PLAYER_STATE::Jump_Landing: {
    }
    break;
    case PLAYER_STATE::Run: {
    }
    break;
    case PLAYER_STATE::RunUturn: {
        RotateTransform();
    }
    break;
    case PLAYER_STATE::RunToIdle: {
    }
    break;
    case PLAYER_STATE::Dash: {
        Rigidbody2D()->SetGravityScale(m_RigidGravityScale);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));
        EnableParticle(false);
    }
    break;
    case PLAYER_STATE::Hit02: {
    }
    break;
    case PLAYER_STATE::Hit01: {
    }
    break;
    case PLAYER_STATE::Fight_To_Idle: {
    }
    break;
    case PLAYER_STATE::Elevator_In: {
    }
    break;
    case PLAYER_STATE::Elevator_End: {
    }
    break;
    case PLAYER_STATE::Elevator_Out: {
    }
    break;
    case PLAYER_STATE::Cinematic: {
    }
    break;
    case PLAYER_STATE::EndBoss: {
    }
    break;
    case PLAYER_STATE::SpawnLobby: {
    }
    break;
    case PLAYER_STATE::TheScythe_Attack: {
        SetHitBox(false);
    }
    break;
    case PLAYER_STATE::TheScythe_AerialAttack: {
        Rigidbody2D()->SetGravityScale(m_RigidGravityScale);
        SetHitBox(false);
    }
    break;
    case PLAYER_STATE::TheScythe_JumpAttack: {
        SetHitBox(false);
    }
    break;
    case PLAYER_STATE::TheScythe_DownAttack: {
        SetHitBox(false);
    }
    break;
    case PLAYER_STATE::Slaymore_UpDown: {
        m_ATK = 10;
        Rigidbody2D()->SetGravityScale(m_RigidGravityScale);
        SetHitBox(false);
    }
    break;
    }
}

void CPlayerScript::Idle()
{
    // �̵� ����
    StopWalking();

    // ���߿� ���ִ� ����
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // �¿� Ű ���ÿ� �������¸� Idle ����
    }
    // Run
    else if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        if (DIRECTION_TYPE::RIGHT == m_Dir)
            ChangeState(PLAYER_STATE::IdleUturn);
        else
            ChangeState(PLAYER_STATE::IdleToRun);

        m_Dir = DIRECTION_TYPE::LEFT;
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        if (DIRECTION_TYPE::LEFT == m_Dir)
            ChangeState(PLAYER_STATE::IdleUturn);
        else
            ChangeState(PLAYER_STATE::IdleToRun);

        m_Dir = DIRECTION_TYPE::RIGHT;
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::IdleToRun()
{
    // �̵�
    Walking();

    // ���߿� ���ִ� ����
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    // Run
    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Run);
    }

    // ���� ��ȯ
    if (DIRECTION_TYPE::LEFT == m_Dir && (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)))
    {
        ChangeState(PLAYER_STATE::IdleUturn);
        m_Dir = DIRECTION_TYPE::RIGHT;
    }
    else if (DIRECTION_TYPE::RIGHT == m_Dir && (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)))
    {
        ChangeState(PLAYER_STATE::IdleUturn);
        m_Dir = DIRECTION_TYPE::LEFT;
    }
    // Ű�� ������ ���� ���¶�� Idle �� ����
    else if (!(KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D)))
    {
        ChangeState(PLAYER_STATE::Idle);
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::IdleUturn()
{
    // �̵� ����
    StopWalking();

    // ���߿� ���ִ� ����
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Idle);
    }

    // ���� ��ȯ
    if (DIRECTION_TYPE::LEFT == m_Dir && KEY_TAP(KEY::D))
    {
        ChangeState(PLAYER_STATE::IdleUturn);
        m_Dir = DIRECTION_TYPE::RIGHT;
    }
    else if (DIRECTION_TYPE::RIGHT == m_Dir && KEY_TAP(KEY::A))
    {
        ChangeState(PLAYER_STATE::IdleUturn);
        m_Dir = DIRECTION_TYPE::LEFT;
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::Jump_Falling()
{
    // �̵� ����
    StopWalking();

    if (m_bOnGround)
        ChangeState(PLAYER_STATE::Jump_Landing);

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // �¿� Ű ���ÿ� �������¸� �̵� X
    }
    // Ű�� ���� ���¶�� �̵�
    else if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        m_Dir = DIRECTION_TYPE::LEFT;
        RotateTransform();
        Walking();
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        m_Dir = DIRECTION_TYPE::RIGHT;
        RotateTransform();
        Walking();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::Jump_Start()
{
    // �̵� ����
    StopWalking();

    static float AirTime = 0.f;

    AirTime += DT;
    Vec2 Vel = Rigidbody2D()->GetVelocity();

    // Space Ű�� ���� �ð���ŭ ���
    if (AirTime < 0.3f && KEY_PRESSED(SPACE))
    {
        Rigidbody2D()->AddForce(Vec2(0.f, m_JumpForce));
    }
    else if (Vel.y < 5.f)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
        AirTime = 0.f;
    }

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // �¿� Ű ���ÿ� �������¸� �̵� X
    }
    // Ű�� ���� ���¶�� �̵�
    else if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        m_Dir = DIRECTION_TYPE::LEFT;
        RotateTransform();
        Walking();
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        m_Dir = DIRECTION_TYPE::RIGHT;
        RotateTransform();
        Walking();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::Jump_Landing()
{
    // �̵� ����
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // �¿� Ű ���ÿ� �������¸� �̵� X
    }
    // Ű�� ���� ���¶�� �̵�
    else if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        m_Dir = DIRECTION_TYPE::LEFT;
        RotateTransform();
        Walking();
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        m_Dir = DIRECTION_TYPE::RIGHT;
        RotateTransform();
        Walking();
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeState(PLAYER_STATE::Jump_Start);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::Run()
{
    // �̵�
    Walking();

    // ���߿� ���ִ� ����
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    // Idle
    if (!(KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D)))
    {
        ChangeState(PLAYER_STATE::RunToIdle);
    }

    // Run ���� �ݴ���� Ű�� ������� Idle
    if ((KEY_PRESSED(KEY::A) && KEY_TAP(KEY::D)) || KEY_PRESSED(KEY::D) && KEY_TAP(KEY::A))
    {
        ChangeState(PLAYER_STATE::RunToIdle);
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::RunUturn()
{
    // ���߿� ���ִ� ����
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Run);
    }

    // ���� ��ȯ
    if (DIRECTION_TYPE::LEFT == m_Dir && KEY_TAP(KEY::D))
    {
        ChangeState(PLAYER_STATE::RunUturn);
        m_Dir = DIRECTION_TYPE::RIGHT;
    }
    else if (DIRECTION_TYPE::RIGHT == m_Dir && KEY_TAP(KEY::A))
    {
        ChangeState(PLAYER_STATE::RunUturn);
        m_Dir = DIRECTION_TYPE::LEFT;
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::RunToIdle()
{
    // �̵� ����
    StopWalking();

    // ���߿� ���ִ� ����
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    // Idle
    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Idle);
    }
    else
    {
        if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
        {
            // �¿�Ű ���ÿ� ������� Idle
        }
        else if (DIRECTION_TYPE::LEFT == m_Dir && (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)))
        {
            ChangeState(PLAYER_STATE::RunUturn);
            m_Dir = DIRECTION_TYPE::RIGHT;
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir && (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)))
        {
            ChangeState(PLAYER_STATE::RunUturn);
            m_Dir = DIRECTION_TYPE::LEFT;
        }
        else if (KEY_PRESSED(KEY::D) || KEY_PRESSED(KEY::A))
        {
            ChangeState(PLAYER_STATE::Run);
        }
    }

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::Dash()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::Hit01()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::Hit02()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::Fight_To_Idle()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);

    // Jump
    if (KEY_TAP(KEY::SPACE))
    {
        ChangeStateToJump();
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    CheckChangeStateToAttack();
}

void CPlayerScript::Elevator_In()
{
    CGameObject* ExitElevator = CGameManagerScript::GetInset()->GetExitElevator();

    Vec3 PlayerPos = Transform()->GetRelativePos();
    Vec3 ElevPos = ExitElevator->Transform()->GetRelativePos();

    PlayerPos.z = 0.f;
    ElevPos.z = 0.f;

    Vec3 Dir = ElevPos - PlayerPos;

    if (Dir.Length() < 10.f)
        return;

    Dir.Normalize();

    PlayerPos = Transform()->GetRelativePos();
    PlayerPos.x += (Dir * 500.f * DT).x;

    Transform()->SetRelativePos(PlayerPos);
}

void CPlayerScript::Elevator_End()
{
}

void CPlayerScript::Elevator_Out()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::Cinematic()
{
}

void CPlayerScript::EndBoss()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::SpawnLobby()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::TheScythe_Attack()
{
    static float PassedTime = 0.f;
    PassedTime += DT;

    static bool bNextAttack = false;

    if (0 == m_AttackCount)
    {
        if (Animator2D()->IsFinish())
        {
            if (bNextAttack)
            {
                // ���� ��ȯ
                if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
                {
                    m_Dir = DIRECTION_TYPE::LEFT;
                    RotateTransform();
                }
                else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
                {
                    m_Dir = DIRECTION_TYPE::RIGHT;
                    RotateTransform();
                }

                m_AttackCount = 1;
                ChangeState(PLAYER_STATE::TheScythe_Attack);
            }
            else
                ChangeState(PLAYER_STATE::Idle);

            PassedTime = 0.f;
            bNextAttack = false;
        }

        // Attack
        if (KEY_TAP(KEY::LBTN))
        {
            bNextAttack = true;
        }
    }
    else if (1 == m_AttackCount)
    {
        if (Animator2D()->IsFinish())
        {
            if (bNextAttack)
            {
                // ���� ��ȯ
                if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
                {
                    m_Dir = DIRECTION_TYPE::LEFT;
                    RotateTransform();
                }
                else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
                {
                    m_Dir = DIRECTION_TYPE::RIGHT;
                    RotateTransform();
                }

                m_AttackCount = 2;
                ChangeState(PLAYER_STATE::TheScythe_Attack);
            }
            else
                ChangeState(PLAYER_STATE::Fight_To_Idle);

            PassedTime = 0.f;
            bNextAttack = false;
        }

        if (PassedTime > 0.2f)
            StopWalking();

        // Attack
        if (KEY_TAP(KEY::LBTN))
        {
            bNextAttack = true;
        }
    }
    else if (2 == m_AttackCount)
    {
        if (Animator2D()->IsFinish())
        {
            if (bNextAttack)
            {
                // ���� ��ȯ
                if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
                {
                    m_Dir = DIRECTION_TYPE::LEFT;
                    RotateTransform();
                }
                else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
                {
                    m_Dir = DIRECTION_TYPE::RIGHT;
                    RotateTransform();
                }

                m_AttackCount = 3;
                ChangeState(PLAYER_STATE::TheScythe_Attack);
            }
            else
                ChangeState(PLAYER_STATE::Fight_To_Idle);

            PassedTime = 0.f;
            bNextAttack = false;
        }

        if (PassedTime > 0.2f)
            StopWalking();

        // Attack
        if (KEY_TAP(KEY::LBTN))
        {
            bNextAttack = true;
        }
    }
    else if (3 == m_AttackCount)
    {
        static bool bShaked = false;

        if (Animator2D()->IsFinish())
        {
            ChangeState(PLAYER_STATE::Idle);
            PassedTime = 0.f;
            bNextAttack = false;
            bShaked = false;
        }

        if (PassedTime > 0.4f)
            StopWalking();

        if (!bShaked && 10 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
        {
            // Camera Shake
            CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
            if (nullptr != pPlayerCamObj)
            {
                CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
                if (nullptr != pScript)
                {
                    pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f);
                    bShaked = true;
                }
            }
        }
    }
    else
    {
        ChangeState(PLAYER_STATE::Idle);
        PassedTime = 0.f;
        bNextAttack = false;
    }

    // Attack �� ���߿� �� �����ϰ�� TheScythe_AerialAttack �� ��ȯ
    if (!m_bOnGround && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::TheScythe_AerialAttack);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::Dash);
    }

    // ���� ����, �ϰ� ����
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::TheScythe_JumpAttack);
    }
    // else if (KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    //{
    //     m_AttackCount = 0;
    //     PassedTime = 0.f;
    //     bNextAttack = false;
    //     ChangeState(PLAYER_STATE::);
    // }
}

void CPlayerScript::TheScythe_AerialAttack()
{
    static float PassedTime = 0.f;
    PassedTime += DT;

    static bool bNextAttack = false;

    if (0 == m_AttackCount)
    {
        if (Animator2D()->IsFinish())
        {
            if (bNextAttack)
            {
                // ���� ��ȯ
                if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
                {
                    m_Dir = DIRECTION_TYPE::LEFT;
                    RotateTransform();
                }
                else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
                {
                    m_Dir = DIRECTION_TYPE::RIGHT;
                    RotateTransform();
                }

                m_AttackCount = 1;
                ChangeState(PLAYER_STATE::TheScythe_AerialAttack);
            }
            else
                ChangeState(PLAYER_STATE::Fight_To_Idle);

            PassedTime = 0.f;
            bNextAttack = false;
        }

        // Attack
        if (KEY_TAP(KEY::LBTN))
        {
            bNextAttack = true;
        }
    }
    else if (1 == m_AttackCount)
    {
        if (Animator2D()->IsFinish())
        {
            if (bNextAttack)
            {
                // ���� ��ȯ
                if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
                {
                    m_Dir = DIRECTION_TYPE::LEFT;
                    RotateTransform();
                }
                else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
                {
                    m_Dir = DIRECTION_TYPE::RIGHT;
                    RotateTransform();
                }

                m_AttackCount = 2;
                ChangeState(PLAYER_STATE::TheScythe_AerialAttack);
            }
            else
                ChangeState(PLAYER_STATE::Fight_To_Idle);

            PassedTime = 0.f;
            bNextAttack = false;
        }

        if (PassedTime > 0.2f)
            StopWalking();

        // Attack
        if (KEY_TAP(KEY::LBTN))
        {
            bNextAttack = true;
        }
    }
    else if (2 == m_AttackCount)
    {
        if (Animator2D()->IsFinish())
        {
            ChangeState(PLAYER_STATE::Fight_To_Idle);
            PassedTime = 0.f;
            bNextAttack = false;
        }

        if (PassedTime > 0.2f)
            StopWalking();
    }
    else
    {
        ChangeState(PLAYER_STATE::Idle);
        PassedTime = 0.f;
        bNextAttack = false;
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::Dash);
    }

    // ���� ����, �ϰ� ����
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::TheScythe_JumpAttack);
    }
    else if (KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::TheScythe_DownAttack);
    }
}

void CPlayerScript::TheScythe_JumpAttack()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::TheScythe_DownAttack()
{
    static bool bShaked = false;

    if (Animator2D()->IsFinish())
    {
        bShaked = false;
        ChangeState(PLAYER_STATE::Idle);
    }

    // Camera Shake
    if (!bShaked && 6 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
        if (nullptr != pPlayerCamObj)
        {
            CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
            if (nullptr != pScript)
            {
                pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f);
                bShaked = true;
            }
        }
    }
}

void CPlayerScript::Slaymore_UpDown()
{
    int CurIdx = Animator2D()->GetCurAnim()->GetCurFrmIdx();
    if (CurIdx >= 5 && CurIdx <= 8)
    {
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));
    }
    else if (CurIdx >= 9)
    {
        Rigidbody2D()->SetGravityScale(m_RigidGravityScale * 2.f);
    }

    static bool bShaked = false;

    if (Animator2D()->IsFinish())
    {
        bShaked = false;
        ChangeState(PLAYER_STATE::Idle);
    }

    // Camera Shake
    if (!bShaked && 13 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        GamePlayStatic::Play2DSound(L"sound\\Player\\PC_Atk_Y_SwordBig_Air_B_01.wav", 1, 0.5f);
        CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
        if (nullptr != pPlayerCamObj)
        {
            CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
            if (nullptr != pScript)
            {
                pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f);
                bShaked = true;
            }
        }
    }
}

void CPlayerScript::RotateTransform()
{
    if (DIRECTION_TYPE::LEFT == m_Dir)
        Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
    else
        Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}

void CPlayerScript::GroundCheck()
{
    // RayCast
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), m_RaycastDist, Vec3(1.f, 0.f, 0.f), false);

    if (Rigidbody2D()->GetVelocity().y <= 0.f) // ���� or ���� ����
    {
        Vec3 origin = Transform()->GetWorldPos();
        RaycastHit2D Hit =
            CPhysics2DMgr::GetInst()->RayCast(Vec2(origin.x, origin.y), Vec2(0.f, -1.f), m_RaycastDist, L"Ground"); // Ground ���̾�� �浹üũ
        if (nullptr != Hit.pCollisionObj)
        {
            m_bOnGround = true;
            m_bJumpAttackActive = true;

            // Hit.Distance; // Player ���� ���� Ground ǥ������� �Ÿ�
            //  TODO �׸��� ó��

            // Bridge �ΰ�� Ȱ��ȭ ó��
            CBridgeScript* BridgeScript = Hit.pCollisionObj->GetScript<CBridgeScript>();
            if (nullptr != BridgeScript)
            {
                // Ư�� ���¿����� ���� Ȱ��ȭ
                bool bForce = PLAYER_STATE::TheScythe_DownAttack == m_State;
                BridgeScript->SetEnabled(true, bForce);
            }
        }
        else
        {
            m_bOnGround = false;
        }
    }
}

void CPlayerScript::ChangeStateToJump()
{
    bool m_bBridge = false;

    if (KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S))
    {
        Vec3 origin = Transform()->GetWorldPos();
        RaycastHit2D Hit =
            CPhysics2DMgr::GetInst()->RayCast(Vec2(origin.x, origin.y), Vec2(0.f, -1.f), m_RaycastDist, L"Ground"); // Ground ���̾�� �浹üũ

        if (nullptr != Hit.pCollisionObj)
        {
            CBridgeScript* BridgeScript = Hit.pCollisionObj->GetScript<CBridgeScript>();
            if (nullptr != BridgeScript)
            {
                m_bBridge = true;
                BridgeScript->SetEnabled(false); // Bridge ��Ȱ��ȭ ó��
            }
        }
    }

    if (m_bBridge)
    {
        m_bOnGround = false;
        ChangeState(PLAYER_STATE::Jump_Falling);
    }
    else
    {
        ChangeState(PLAYER_STATE::Jump_Start);
    }
}

void CPlayerScript::CheckChangeStateToAttack()
{
    // ===================
    // Scythe
    // ===================
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        if (m_Scythe == PLAYER_WEAPON_SCYTHE::TheScythe)
            ChangeState(PLAYER_STATE::TheScythe_JumpAttack);
    }
    else if (!m_bOnGround && KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    {
        if (m_Scythe == PLAYER_WEAPON_SCYTHE::TheScythe)
            ChangeState(PLAYER_STATE::TheScythe_DownAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        if (m_Scythe == PLAYER_WEAPON_SCYTHE::TheScythe)
        {
            if (m_State == PLAYER_STATE::Fight_To_Idle)
            {
                ++m_AttackCount;
            }

            if (m_bOnGround)
                ChangeState(PLAYER_STATE::TheScythe_Attack);
            else
                ChangeState(PLAYER_STATE::TheScythe_AerialAttack);
        }
    }

    // ===================
    // Cloak
    // ===================
    bool bCloakInput = KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::NUM8);
    if (bCloakInput)
    {
        if (m_Cloak == PLAYER_WEAPON_CLOAK::Slaymore)
        {
            ChangeState(PLAYER_STATE::Slaymore_UpDown);
        }
    }

    // ===================
    // Spell
    // ===================
    bool bSpellInput = KEY_TAP(KEY::MBTN) || KEY_TAP(KEY::NUM9);
    if (bSpellInput)
    {
        if (m_Spell == PLAYER_WEAPON_SPELL::WizzalchBarrage)
        {
            if (m_CurMana >= 15)
            {
                m_CurMana -= 15;
                m_ManaDelay = 0.f;

                CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
                int EffectIdx = pCurLevel->FindLayerIndexByName(L"Effect");
                if (-1 == EffectIdx)
                    EffectIdx = 0;

                CGameObject* pLightningObj = m_pLightningSpawner02Pref->Instantiate();
                pLightningObj->Transform()->SetRelativePos(Transform()->GetWorldPos() + Vec3(0.f, 300.f, -250.f));

                GamePlayStatic::SpawnGameObject(pLightningObj, EffectIdx);
                GamePlayStatic::Play2DSound(L"sound\\FX\\PC_FX_Spawn_Lightning_01.wav", 1, 0.5f);

                // Camera Shake
                CGameObject* pPlayerCamObj = pCurLevel->FindObjectByName(L"PlayerCamera");
                if (nullptr != pPlayerCamObj)
                {
                    CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
                    if (nullptr != pScript)
                    {
                        pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f);
                    }
                }
            }
        }
    }
}

void CPlayerScript::SetHitBox(bool _Enable, const wstring& _HitBoxName)
{
    const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
    for (size_t i = 0; i < vecChild.size(); i++)
    {
        CPlayerHitBoxScript* pHitBox = vecChild[i]->GetScript<CPlayerHitBoxScript>();
        if (nullptr == pHitBox)
            continue;

        // _HitBoxName �� �Է� ���� ���� ��� ��� HitBox�� ����
        // _HitBoxName �� �Է� �Ȱ�� �ش� �̸��� HitBox�� ����
        if (!_HitBoxName.empty() && pHitBox->GetOwner()->GetName() != _HitBoxName)
            continue;

        pHitBox->SetEnabled(_Enable);
    }
}

void CPlayerScript::EnableParticle(bool _bEnable)
{
    const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
    for (size_t i = 0; i < vecChild.size(); i++)
    {
        if (L"Particle" != vecChild[i]->GetName())
            continue;

        if (nullptr != vecChild[i]->ParticleSystem())
        {
            vecChild[i]->ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, _bEnable);
        }
    }
}

void CPlayerScript::Walking()
{
    Vec2 vel = Rigidbody2D()->GetVelocity();

    if (DIRECTION_TYPE::LEFT == m_Dir)
        vel.x = -m_Speed;
    else
        vel.x = m_Speed;

    Rigidbody2D()->SetVelocity(vel);
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();
    if (LayerIdx >= 0)
    {
        if (L"Ground" == pCurLevel->GetLayer(LayerIdx)->GetName())
            m_bOnGround = false;
    }

    CBridgeScript* BridgeScript = _OtherCollider->GetOwner()->GetScript<CBridgeScript>();
    if (nullptr != BridgeScript)
    {
        BridgeScript->SetEnabled(false); // Bridge ��Ȱ��ȭ ó��
    }
}

void CPlayerScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Scythe, sizeof(PLAYER_WEAPON_SCYTHE), 1, _File);
    fwrite(&m_Cloak, sizeof(PLAYER_WEAPON_CLOAK), 1, _File);
    fwrite(&m_Spell, sizeof(PLAYER_WEAPON_SPELL), 1, _File);
    fwrite(&m_MaxLife, sizeof(int), 1, _File);
    fwrite(&m_MaxMana, sizeof(int), 1, _File);
    fwrite(&m_CurLife, sizeof(int), 1, _File);
    fwrite(&m_CurMana, sizeof(int), 1, _File);
    fwrite(&m_ATK, sizeof(int), 1, _File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_JumpImpulse, sizeof(float), 1, _File);
    fwrite(&m_JumpForce, sizeof(float), 1, _File);
    fwrite(&m_DashImpulse, sizeof(float), 1, _File);
    fwrite(&m_AttackImpulse, sizeof(float), 1, _File);
    fwrite(&m_RaycastDist, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Scythe, sizeof(PLAYER_WEAPON_SCYTHE), 1, _File);
    fread(&m_Cloak, sizeof(PLAYER_WEAPON_CLOAK), 1, _File);
    fread(&m_Spell, sizeof(PLAYER_WEAPON_SPELL), 1, _File);
    fread(&m_MaxLife, sizeof(int), 1, _File);
    fread(&m_MaxMana, sizeof(int), 1, _File);
    fread(&m_CurLife, sizeof(int), 1, _File);
    fread(&m_CurMana, sizeof(int), 1, _File);
    fread(&m_ATK, sizeof(int), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_JumpImpulse, sizeof(float), 1, _File);
    fread(&m_JumpForce, sizeof(float), 1, _File);
    fread(&m_DashImpulse, sizeof(float), 1, _File);
    fread(&m_AttackImpulse, sizeof(float), 1, _File);
    fread(&m_RaycastDist, sizeof(float), 1, _File);
}