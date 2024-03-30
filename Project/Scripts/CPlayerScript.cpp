#include "pch.h"
#include "CPlayerScript.h"
#include <Engine\\CPhysics2DMgr.h>
#include <Engine\\CLevelMgr.h>

#include <Engine\\CLevel.h>
#include <Engine\\CAnim.h>
#include "CPlayerCameraScript.h"
#include "CPlayerHitBoxScript.h"

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_State(PLAYER_STATE::Idle)
    , m_Dir(DIRECTION_TYPE::RIGHT)
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
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxLife, "Max Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxMana, "Max Mana");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurLife, "Current Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurMana, "Current Mana");
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
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DLightMtrl"));

        Transform()->SetRelativeScale(Vec3(1024.f, 1024.f, 1.f));
    }

    MeshRender()->CreateDynamicMaterial();

    // 애니메이션 로딩
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

        // The Scythe
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

        /*Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Acquisition.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Acquisition_Curse.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BookAttack_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BookAttack_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BookAttack_03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BossRitual_Intro.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Bump.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Combo_Stand.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_01_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_02_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_03_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboEvilHands.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboEvilHands_Up.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatana.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatanas_Ult.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatanas_Ult_Init.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatanas_Ult_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_01_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_02_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_03_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_04.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_04_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_Jump.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Concentrate.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Concentrate_Start.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Disappear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_EvilBirds.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_EvilBirds_Up.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Glide.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_OneWayDown.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_OneWayUp.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Ritual_End_Boss.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Spawn_Lobby.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Teleport.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Waiting.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Wallgrab_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ZipUp.anim");*/
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();
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
    case PLAYER_STATE::ComboMove:
        ComboMove();
        break;
    case PLAYER_STATE::ComboAerial:
        ComboAerial();
        break;
    case PLAYER_STATE::JumpAttack:
        JumpAttack();
        break;
    case PLAYER_STATE::DownAttack:
        DownAttack();
        break;
    }

    m_DashPassedTime += DT;

    RayCast();
}

void CPlayerScript::ChangeState(PLAYER_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CPlayerScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (m_State == PLAYER_STATE::Dash || m_State == PLAYER_STATE::DownAttack || m_State == PLAYER_STATE::JumpAttack)
        return;

    m_CurLife -= _DamageAmount;

    Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
    Force.Normalize();
    Force *= 100.f;
    Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

    ChangeState(PLAYER_STATE::Hit02);
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
        m_RigidGravityScale = Rigidbody2D()->GetGravityScale();
        Rigidbody2D()->SetGravityScale(0.f);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));

        if (DIRECTION_TYPE::LEFT == m_Dir)
            Rigidbody2D()->AddForce(Vec2(-m_DashImpulse, 0.f), ForceMode2D::Impulse);
        else
            Rigidbody2D()->AddForce(Vec2(m_DashImpulse, 0.f), ForceMode2D::Impulse);

        m_DashPassedTime = 0.f;
    }
    break;
    case PLAYER_STATE::Hit01: {
        Animator2D()->Play(L"LD_Hit01", false);
    }
    break;
    case PLAYER_STATE::Hit02: {
        Animator2D()->Play(L"LD_Hit02", false);
    }
    break;
    case PLAYER_STATE::Fight_To_Idle: {
        Animator2D()->Play(L"LD_Fight_To_Idle", false);
    }
    break;
    case PLAYER_STATE::Elevator_In: {
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
    case PLAYER_STATE::ComboMove: {
        if (0 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove01", false);
            OnHitBox(true, L"HitBox1");
        }
        else if (1 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove02", false);
            OnHitBox(false, L"HitBox1");
            OnHitBox(true, L"HitBox2");
        }
        else if (2 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove03", false);
            OnHitBox(false, L"HitBox2");
            OnHitBox(true, L"HitBox1");
        }
        else if (3 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove04", false);
            OnHitBox(false, L"HitBox1");
            OnHitBox(true, L"HitBox3");
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
    case PLAYER_STATE::ComboAerial: {
        if (0 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboStand1", false);
            OnHitBox(true, L"HitBox3");
        }
        else if (1 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboMove01", false);
            OnHitBox(false, L"HitBox3");
            OnHitBox(true, L"HitBox1");
        }
        else if (2 == m_AttackCount)
        {
            Animator2D()->Play(L"LD_ComboStand2", false);
            OnHitBox(false, L"HitBox1");
            OnHitBox(true, L"HitBox3");
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
    case PLAYER_STATE::JumpAttack: {
        Animator2D()->Play(L"LD_Special2", false);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));
        Rigidbody2D()->AddForce(Vec2(0.f, m_JumpImpulse * 1.5f), ForceMode2D::Impulse);

        m_bJumpAttackActive = false;

        OnHitBox(true, L"HitBox2");
    }
    break;
    case PLAYER_STATE::DownAttack: {
        Animator2D()->Play(L"LD_Crush", false);
        Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f));
        Rigidbody2D()->AddForce(Vec2(0.f, -m_JumpImpulse * 3.f), ForceMode2D::Impulse);

        OnHitBox(true, L"HitBox2");
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
    case PLAYER_STATE::ComboMove: {
        OnHitBox(false);
    }
    break;
    case PLAYER_STATE::ComboAerial: {
        Rigidbody2D()->SetGravityScale(m_RigidGravityScale);
        OnHitBox(false);
    }
    break;
    case PLAYER_STATE::JumpAttack: {
        OnHitBox(false);
    }
    break;
    case PLAYER_STATE::DownAttack: {
        OnHitBox(false);
    }
    break;
    }
}

void CPlayerScript::Idle()
{
    // 이동 제한
    StopWalking();

    // 공중에 떠있는 상태
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // 좌우 키 동시에 누른상태면 Idle 상태
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
        ChangeState(PLAYER_STATE::Jump_Start);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
}

void CPlayerScript::IdleToRun()
{
    // 이동
    Walking();

    // 공중에 떠있는 상태
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    // Run
    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Run);
    }

    // 방향 전환
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
    // 키가 눌리지 않은 상태라면 Idle 로 복귀
    else if (!(KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D)))
    {
        ChangeState(PLAYER_STATE::Idle);
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
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
}

void CPlayerScript::IdleUturn()
{
    // 이동 제한
    StopWalking();

    // 공중에 떠있는 상태
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Idle);
    }

    // 방향 전환
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
        ChangeState(PLAYER_STATE::Jump_Start);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
}

void CPlayerScript::Jump_Falling()
{
    // 이동 제한
    StopWalking();

    if (m_bOnGround)
        ChangeState(PLAYER_STATE::Jump_Landing);

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // 좌우 키 동시에 누른상태면 이동 X
    }
    // 키를 누른 상태라면 이동
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
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::DownAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboAerial);
    }
}

void CPlayerScript::Jump_Start()
{
    // 이동 제한
    StopWalking();

    static float AirTime = 0.f;

    AirTime += DT;
    Vec2 Vel = Rigidbody2D()->GetVelocity();

    // Space 키를 누른 시간만큼 상승
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
        // 좌우 키 동시에 누른상태면 이동 X
    }
    // 키를 누른 상태라면 이동
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
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::DownAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboAerial);
    }
}

void CPlayerScript::Jump_Landing()
{
    // 이동 제한
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);

    if (KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
    {
        // 좌우 키 동시에 누른상태면 이동 X
    }
    // 키를 누른 상태라면 이동
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
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
}

void CPlayerScript::Run()
{
    // 이동
    Walking();

    // 공중에 떠있는 상태
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    // Idle
    if (!(KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D)))
    {
        ChangeState(PLAYER_STATE::RunToIdle);
    }

    // Run 도중 반대반향 키를 누른경우 Idle
    if ((KEY_PRESSED(KEY::A) && KEY_TAP(KEY::D)) || KEY_PRESSED(KEY::D) && KEY_TAP(KEY::A))
    {
        ChangeState(PLAYER_STATE::RunToIdle);
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
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
}

void CPlayerScript::RunUturn()
{
    // 공중에 떠있는 상태
    if (!m_bOnGround)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(PLAYER_STATE::Run);
    }

    // 방향 전환
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
        ChangeState(PLAYER_STATE::Jump_Start);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
}

void CPlayerScript::RunToIdle()
{
    // 이동 제한
    StopWalking();

    // 공중에 떠있는 상태
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
            // 좌우키 동시에 누른경우 Idle
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
        ChangeState(PLAYER_STATE::Jump_Start);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::ComboMove);
    }
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
        ChangeState(PLAYER_STATE::Jump_Start);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        ChangeState(PLAYER_STATE::Dash);
    }

    // Attack
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_TAP(KEY::LBTN))
    {
        ++m_AttackCount;
        if (m_bOnGround)
            ChangeState(PLAYER_STATE::ComboMove);
        else
            ChangeState(PLAYER_STATE::ComboAerial);
    }
}

void CPlayerScript::Elevator_In()
{
}

void CPlayerScript::Elevator_End()
{
}

void CPlayerScript::Elevator_Out()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::ComboMove()
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
                // 방향 전환
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
                ChangeState(PLAYER_STATE::ComboMove);
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
                // 방향 전환
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
                ChangeState(PLAYER_STATE::ComboMove);
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
                // 방향 전환
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
                ChangeState(PLAYER_STATE::ComboMove);
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
                    pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f, 100.f);
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

    // Attack 중 공중에 뜬 상태일경우 ComboAerial 로 전환
    if (!m_bOnGround && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::ComboAerial);
    }

    // Dash
    if (m_DashPassedTime > m_DashCoolTime && KEY_TAP(KEY::LSHIFT))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::Dash);
    }

    // 점프 & 하강 공격
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    // else if (KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    //{
    //     m_AttackCount = 0;
    //     PassedTime = 0.f;
    //     bNextAttack = false;
    //     ChangeState(PLAYER_STATE::);
    // }
}

void CPlayerScript::ComboAerial()
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
                // 방향 전환
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
                ChangeState(PLAYER_STATE::ComboAerial);
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
                // 방향 전환
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
                ChangeState(PLAYER_STATE::ComboAerial);
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

    // 점프 & 하강 공격
    if (m_bJumpAttackActive && KEY_PRESSED(KEY::W) && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::JumpAttack);
    }
    else if (KEY_PRESSED(KEY::S) && KEY_TAP(KEY::LBTN))
    {
        m_AttackCount = 0;
        PassedTime = 0.f;
        bNextAttack = false;
        ChangeState(PLAYER_STATE::DownAttack);
    }
}

void CPlayerScript::JumpAttack()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);
}

void CPlayerScript::DownAttack()
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
                pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f, 100.f);
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

void CPlayerScript::RayCast()
{
    // RayCast
    if (Rigidbody2D()->GetVelocity().y <= 0.f) // 낙하 or 정지 상태
    {
        Vec3 origin = Transform()->GetWorldPos();
        RaycastHit2D Hit =
            CPhysics2DMgr::GetInst()->RayCast(Vec2(origin.x, origin.y), Vec2(0.f, -1.f), m_RaycastDist, L"Ground"); // Ground 레이어와 충돌체크
        GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), m_RaycastDist, Vec3(1.f, 0.f, 0.f), false);

        if (nullptr != Hit.pCollisionObj)
        {
            m_bOnGround = true;
            m_bJumpAttackActive = true;

            Hit.Distance; // Player 중점 에서 Ground 표면까지의 거리
            // TODO 그림자 처리
        }
        else
        {
            m_bOnGround = false;
        }
    }
}

void CPlayerScript::OnHitBox(bool _Enable, const wstring& _HitBoxName)
{
    const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
    for (size_t i = 0; i < vecChild.size(); i++)
    {
        CPlayerHitBoxScript* pHitBox = vecChild[i]->GetScript<CPlayerHitBoxScript>();
        if (nullptr == pHitBox)
            continue;

        // _HitBoxName 이 입력되지않은 경우 모든 HitBox에 적용
        // _HitBoxName 이 입력 된경우 해당 이름의 HitBox만 적용
        if (!_HitBoxName.empty() && pHitBox->GetOwner()->GetName() != _HitBoxName)
            continue;

        pHitBox->SetEnable(_Enable);
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
