#include "pch.h"
#include "CPlayerScript.h"
#include <Engine\\CLevelMgr.h>
#include <Engine\\CLevel.h>

#include <Engine\\CAnim.h>

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_State(PLAYER_STATE::Idle)
    , m_Dir(DIRECTION_TYPE::LEFT)
    , m_Speed(10.f)
    , m_JumpImpulse(1.f)
    , m_JumpForce(1.f)
    , m_bOnGround(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpImpulse, "Jump Impulse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpForce, "Jump Force");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
    if (MeshRender())
        MeshRender()->CreateDynamicMaterial();

    if (Animator2D())
    {
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Acquisition.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Acquisition_Curse.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_AerialDownAttack.anim");
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
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Dash.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Disappear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_End.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_Enter.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_Out.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_EvilBirds.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_EvilBirds_Up.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Glide.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Hit.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_IdleToRun.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_IdleUturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Falling.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Landing.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Start.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_JumpingAttack.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_OneWayDown.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_OneWayUp.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Ritual_End_Boss.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Run.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_RunToIdle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_RunUturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Spawn_Lobby.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Teleport.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Waiting.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Wallgrab_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ZipUp.anim");
    }

    ChangeState(PLAYER_STATE::Idle);
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
    case PLAYER_STATE::ComboMove:
        ComboMove();
        break;
    case PLAYER_STATE::ComboMove_Rest:
        ComboMove_Rest();
        break;
    case PLAYER_STATE::ComboAerial:
        ComboAerial();
        break;
    case PLAYER_STATE::ComboAerial_Rest:
        ComboAerial_Rest();
        break;
    case PLAYER_STATE::JumpingAttack:
        JumpingAttack();
        break;
    case PLAYER_STATE::AerialDownAttack:
        AerialDownAttack();
        break;
    case PLAYER_STATE::CapeAttack:
        CapeAttack();
        break;
    case PLAYER_STATE::Dash:
        Dash();
        break;
    case PLAYER_STATE::Hit:
        Hit();
        break;
    case PLAYER_STATE::EnterElavator:
        EnterElavator();
        break;
    case PLAYER_STATE::ExitElavator:
        ExitElavator();
        break;
    case PLAYER_STATE::Acquisition:
        Acquisition();
        break;
    case PLAYER_STATE::PowerUp:
        PowerUp();
        break;
    case PLAYER_STATE::UltAttack:
        UltAttack();
        break;
    case PLAYER_STATE::UltAttack_Rest:
        UltAttack_Rest();
        break;
    }

    // 테스트 코드
    if (KEY_TAP(KEY::T))
    {
        ChangeState(PLAYER_STATE::Idle);
    }
}

void CPlayerScript::ChangeState(PLAYER_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CPlayerScript::EnterState()
{
    switch (m_State)
    {
    case PLAYER_STATE::Idle: {
        Animator2D()->Play(L"LD_Idle");
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
    case PLAYER_STATE::ComboMove: {
        Animator2D()->Play(L"LD_ComboMove", false);
    }
    break;
    case PLAYER_STATE::ComboMove_Rest: {
        Animator2D()->Play(L"LD_ComboMove_Rest", false);
    }
    break;
    case PLAYER_STATE::ComboAerial: {
        Animator2D()->Play(L"LD_ComboAerial", false);
    }
    break;
    case PLAYER_STATE::ComboAerial_Rest: {
        Animator2D()->Play(L"LD_ComboAerial_Rest", false);
    }
    break;
    case PLAYER_STATE::JumpingAttack: {
        Animator2D()->Play(L"LD_JumpingAttack", false);
    }
    break;
    case PLAYER_STATE::AerialDownAttack: {
        Animator2D()->Play(L"LD_AerialDownAttack", false);
    }
    break;
    case PLAYER_STATE::CapeAttack: {
    }
    break;
    case PLAYER_STATE::Dash: {
        Animator2D()->Play(L"LD_Dash", false);
    }
    break;
    case PLAYER_STATE::Hit: {
        Animator2D()->Play(L"LD_Hit", false);
    }
    break;
    case PLAYER_STATE::EnterElavator: {
        Animator2D()->Play(L"LD_EnterElavator", false);
    }
    break;
    case PLAYER_STATE::ExitElavator: {
        Animator2D()->Play(L"LD_ExitElavator", false);
    }
    break;
    case PLAYER_STATE::Acquisition: {
        Animator2D()->Play(L"LD_Acquisition", false);
    }
    break;
    case PLAYER_STATE::PowerUp: {
        Animator2D()->Play(L"LD_PowerUp", false);
    }
    break;
    case PLAYER_STATE::UltAttack: {
        Animator2D()->Play(L"LD_UltAttack", false);
    }
    break;
    case PLAYER_STATE::UltAttack_Rest: {
        Animator2D()->Play(L"LD_UltAttack_Rest", false);
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
    case PLAYER_STATE::ComboMove: {
    }
    break;
    case PLAYER_STATE::ComboMove_Rest: {
    }
    break;
    case PLAYER_STATE::ComboAerial: {
    }
    break;
    case PLAYER_STATE::ComboAerial_Rest: {
    }
    break;
    case PLAYER_STATE::JumpingAttack: {
    }
    break;
    case PLAYER_STATE::AerialDownAttack: {
    }
    break;
    case PLAYER_STATE::CapeAttack: {
    }
    break;
    case PLAYER_STATE::Dash: {
    }
    break;
    case PLAYER_STATE::Hit: {
    }
    break;
    case PLAYER_STATE::EnterElavator: {
    }
    break;
    case PLAYER_STATE::ExitElavator: {
    }
    break;
    case PLAYER_STATE::Acquisition: {
    }
    break;
    case PLAYER_STATE::PowerUp: {
    }
    break;
    case PLAYER_STATE::UltAttack: {
    }
    break;
    case PLAYER_STATE::UltAttack_Rest: {
    }
    break;
    }
}

void CPlayerScript::Idle()
{
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
}

void CPlayerScript::IdleToRun()
{
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

    // 이동
    MoveTransform();
}

void CPlayerScript::IdleUturn()
{
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
}

void CPlayerScript::Jump_Falling()
{
    if (m_bOnGround)
        ChangeState(PLAYER_STATE::Jump_Landing);

    // 키를 누른 상태라면 이동
    if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        m_Dir = DIRECTION_TYPE::LEFT;
        RotateTransform();
        MoveTransform();
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        m_Dir = DIRECTION_TYPE::RIGHT;
        RotateTransform();
        MoveTransform();
    }
}

void CPlayerScript::Jump_Start()
{
    static float AirTime = 0.f;

    AirTime += DT;
    Vec2 Vel = Rigidbody2D()->GetVelocity();

    if (AirTime < 0.3f && KEY_PRESSED(SPACE))
    {
        Rigidbody2D()->AddForce(Vec2(0.f, m_JumpForce));
    }
    else if (Vel.y < 5.f)
    {
        ChangeState(PLAYER_STATE::Jump_Falling);
        AirTime = 0.f;
    }

    // 키를 누른 상태라면 이동
    if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        m_Dir = DIRECTION_TYPE::LEFT;
        RotateTransform();
        MoveTransform();
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        m_Dir = DIRECTION_TYPE::RIGHT;
        RotateTransform();
        MoveTransform();
    }
}

void CPlayerScript::Jump_Landing()
{
    if (Animator2D()->IsFinish())
        ChangeState(PLAYER_STATE::Idle);

    // 키를 누른 상태라면 이동
    if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        m_Dir = DIRECTION_TYPE::LEFT;
        RotateTransform();
        MoveTransform();
    }
    else if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        m_Dir = DIRECTION_TYPE::RIGHT;
        RotateTransform();
        MoveTransform();
    }
}

void CPlayerScript::Run()
{
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

    // 이동
    MoveTransform();
}

void CPlayerScript::RunUturn()
{
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
}

void CPlayerScript::RunToIdle()
{
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
}

void CPlayerScript::ComboMove()
{
}

void CPlayerScript::ComboMove_Rest()
{
}

void CPlayerScript::ComboAerial()
{
}

void CPlayerScript::ComboAerial_Rest()
{
}

void CPlayerScript::JumpingAttack()
{
}

void CPlayerScript::AerialDownAttack()
{
}

void CPlayerScript::CapeAttack()
{
}

void CPlayerScript::Dash()
{
}

void CPlayerScript::Hit()
{
}

void CPlayerScript::EnterElavator()
{
}

void CPlayerScript::ExitElavator()
{
}

void CPlayerScript::Acquisition()
{
}

void CPlayerScript::PowerUp()
{
}

void CPlayerScript::UltAttack()
{
}

void CPlayerScript::UltAttack_Rest()
{
}

void CPlayerScript::RotateTransform()
{
    if (DIRECTION_TYPE::LEFT == m_Dir)
        Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
    else
        Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}

void CPlayerScript::MoveTransform()
{
    Vec3 pos = Transform()->GetRelativePos();

    if (DIRECTION_TYPE::LEFT == m_Dir)
        pos.x -= m_Speed * DT;
    else
        pos.x += m_Speed * DT;

    Transform()->SetRelativePos(pos);
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();
    if (LayerIdx >= 0)
    {
        if (L"Ground" == pCurLevel->GetLayer(LayerIdx)->GetName())
            m_bOnGround = true;
    }
}

void CPlayerScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();
    if (LayerIdx >= 0)
    {
        if (L"Ground" == pCurLevel->GetLayer(LayerIdx)->GetName())
            m_bOnGround = true;
    }
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
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_JumpImpulse, sizeof(float), 1, _File);
    fwrite(&m_JumpForce, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_JumpImpulse, sizeof(float), 1, _File);
    fread(&m_JumpForce, sizeof(float), 1, _File);
}
