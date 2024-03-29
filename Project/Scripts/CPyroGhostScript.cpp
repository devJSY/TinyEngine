#include "pch.h"
#include "CPyroGhostScript.h"

#include <Engine\\components.h>

CPyroGhostScript::CPyroGhostScript()
    : CScript(PYROGHOSTSCRIPT)
    , m_State(PYROGHOST_STATE::Hide)
    , m_Dir(DIRECTION_TYPE::RIGHT)
    , m_Life(50)
    , m_Speed(5)
    , m_ATK(7)
    , m_AttackRange(100.f)
    , m_PassedTime(0.f)
    , m_pTarget(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_Life, "Life");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ATK, "ATK");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackRange, "Attack Range");
}

CPyroGhostScript::CPyroGhostScript(const CPyroGhostScript& origin)
    : CScript(origin)
    , m_State(origin.m_State)
    , m_Dir(origin.m_Dir)
    , m_Life(origin.m_Life)
    , m_Speed(origin.m_Speed)
    , m_ATK(origin.m_ATK)
    , m_AttackRange(origin.m_AttackRange)
    , m_PassedTime(0.f)
    , m_pTarget(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_Life, "Life");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ATK, "ATK");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackRange, "Attack Range");
}

CPyroGhostScript::~CPyroGhostScript()
{
}

void CPyroGhostScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DLightMtrl"));

        Transform()->SetRelativeScale(Vec3(512.f, 512.f, 1.f));
    }

    MeshRender()->CreateDynamicMaterial();

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_APPEAR.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_ATTACK.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_hit_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_Hit_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_IDLE.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_spotted.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Enemy_Ghost_Woman\\Enemy_Ghost_Woman_UTURN.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();
}

void CPyroGhostScript::tick()
{
    // FSM
    switch (m_State)
    {
    case PYROGHOST_STATE::Idle:
        Idle();
        break;
    case PYROGHOST_STATE::Hide:
        Hide();
        break;
    case PYROGHOST_STATE::Appear:
        Appear();
        break;
    case PYROGHOST_STATE::Attack:
        Attack();
        break;
    case PYROGHOST_STATE::Trace:
        Trace();
        break;
    case PYROGHOST_STATE::Uturn:
        Uturn();
        break;
    case PYROGHOST_STATE::Spotted:
        Spotted();
        break;
    case PYROGHOST_STATE::Hit1:
        Hit1();
        break;
    case PYROGHOST_STATE::Hit2:
        Hit2();
        break;
    case PYROGHOST_STATE::Death:
        Death();
        break;
    }
}

void CPyroGhostScript::ChangeState(PYROGHOST_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CPyroGhostScript::EnterState()
{
    switch (m_State)
    {
    case PYROGHOST_STATE::Idle: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_IDLE");
    }
    break;
    case PYROGHOST_STATE::Hide: {
    }
    break;
    case PYROGHOST_STATE::Appear: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_APPEAR", false);
    }
    break;
    case PYROGHOST_STATE::Attack: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_ATTACK", false);
    }
    break;
    case PYROGHOST_STATE::Trace: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_IDLE");
    }
    break;
    case PYROGHOST_STATE::Uturn: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_UTURN", false);
    }
    break;
    case PYROGHOST_STATE::Spotted: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_spotted", false);
    }
    break;
    case PYROGHOST_STATE::Hit1: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_hit_01", false);
    }
    break;
    case PYROGHOST_STATE::Hit2: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_Hit_02", false);
    }
    break;
    case PYROGHOST_STATE::Death: {
        Animator2D()->Play(L"Enemy_Ghost_Woman_Death", false);
    }
    break;
    }
}

void CPyroGhostScript::ExitState()
{
    switch (m_State)
    {
    case PYROGHOST_STATE::Idle: {
        m_PassedTime = 0.f;
    }
    break;
    case PYROGHOST_STATE::Hide: {
    }
    break;
    case PYROGHOST_STATE::Appear: {
    }
    break;
    case PYROGHOST_STATE::Attack: {
    }
    break;
    case PYROGHOST_STATE::Trace: {
    }
    break;
    case PYROGHOST_STATE::Uturn: {
        RotateTransform();
    }
    break;
    case PYROGHOST_STATE::Spotted: {
    }
    break;
    case PYROGHOST_STATE::Hit1: {
    }
    break;
    case PYROGHOST_STATE::Hit2: {
    }
    break;
    case PYROGHOST_STATE::Death: {
    }
    break;
    }
}

void CPyroGhostScript::Idle()
{
    Walking();

    m_PassedTime += DT;

    if (m_PassedTime > 0.5f)
    {
        // 방향 전환
        if (DIRECTION_TYPE::LEFT == m_Dir)
        {
            ChangeState(PYROGHOST_STATE::Uturn);
            m_Dir = DIRECTION_TYPE::RIGHT;
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir)
        {
            ChangeState(PYROGHOST_STATE::Uturn);
            m_Dir = DIRECTION_TYPE::LEFT;
        }

        m_PassedTime = 0.f;
    }
}

void CPyroGhostScript::Hide()
{
    StopWalking();
}

void CPyroGhostScript::Appear()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::Trace()
{
    Walking();

    Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
    Vec3 pos = Transform()->GetWorldPos();

    TargetPos.z = 0.f;
    pos.z = 0.f;

    Vec3 Dist = TargetPos - pos;

    if (Dist.Length() < m_AttackRange)
    {
        ChangeState(PYROGHOST_STATE::Attack);
    }

    // 방향 전환
    if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
    {
        ChangeState(PYROGHOST_STATE::Uturn);
        m_Dir = DIRECTION_TYPE::RIGHT;
    }
    else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
    {
        ChangeState(PYROGHOST_STATE::Uturn);
        m_Dir = DIRECTION_TYPE::LEFT;
    }
}

void CPyroGhostScript::Attack()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        if (nullptr != m_pTarget)
            ChangeState(PYROGHOST_STATE::Trace);
        else
            ChangeState(PYROGHOST_STATE::Idle);
    }
}

void CPyroGhostScript::Uturn()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        if (nullptr != m_pTarget)
            ChangeState(PYROGHOST_STATE::Trace);
        else
            ChangeState(PYROGHOST_STATE::Idle);
    }
}

void CPyroGhostScript::Spotted()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        if (nullptr != m_pTarget)
            ChangeState(PYROGHOST_STATE::Trace);
        else
            ChangeState(PYROGHOST_STATE::Idle);
    }
}

void CPyroGhostScript::Hit1()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        if (nullptr != m_pTarget)
            ChangeState(PYROGHOST_STATE::Trace);
        else
            ChangeState(PYROGHOST_STATE::Idle);
    }
}

void CPyroGhostScript::Hit2()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        if (nullptr != m_pTarget)
            ChangeState(PYROGHOST_STATE::Trace);
        else
            ChangeState(PYROGHOST_STATE::Idle);
    }
}

void CPyroGhostScript::Death()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CPyroGhostScript::Walking()
{
    Vec2 vel = Rigidbody2D()->GetVelocity();

    if (DIRECTION_TYPE::LEFT == m_Dir)
        vel.x = -m_Speed;
    else
        vel.x = m_Speed;

    Rigidbody2D()->SetVelocity(vel);
}

void CPyroGhostScript::RotateTransform()
{
    if (DIRECTION_TYPE::LEFT == m_Dir)
        Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
    else
        Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}

void CPyroGhostScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CPyroGhostScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CPyroGhostScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
}

void CPyroGhostScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    m_pTarget = _OtherCollider->GetOwner();

    if (PYROGHOST_STATE::Hide == m_State)
        ChangeState(PYROGHOST_STATE::Appear);
    else
        ChangeState(PYROGHOST_STATE::Trace);
}

void CPyroGhostScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CPyroGhostScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    m_pTarget = nullptr;
    ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Life, sizeof(int), 1, _File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_ATK, sizeof(float), 1, _File);
    fwrite(&m_AttackRange, sizeof(float), 1, _File);
}

void CPyroGhostScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Life, sizeof(int), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_ATK, sizeof(float), 1, _File);
    fread(&m_AttackRange, sizeof(float), 1, _File);
}
