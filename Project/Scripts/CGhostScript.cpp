#include "pch.h"
#include "CGhostScript.h"

#include "CPlayerCameraScript.h"
#include <Engine\\components.h>

CGhostScript::CGhostScript()
    : CEnemyScript(GHOSTSCRIPT)
    , m_State(GHOST_STATE::Waiting)
    , m_PassedTime(0.f)
    , m_PatrolDuration(1.5f)
{
    m_CurLife = m_MaxLife = 50;
    m_Speed = 5;
    m_ATK = 5;
    m_AttackRange = 200.f;

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PatrolDuration, "Patrol Duration");
}

CGhostScript::CGhostScript(const CGhostScript& origin)
    : CEnemyScript(origin)
    , m_State(origin.m_State)
    , m_PassedTime(0.f)
    , m_PatrolDuration(origin.m_PatrolDuration)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PatrolDuration, "Patrol Duration");
}

CGhostScript::~CGhostScript()
{
}

void CGhostScript::begin()
{
    CEnemyScript::begin();

    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DGlowMtrl"));

        Transform()->SetRelativeScale(Vec3(512.f, 512.f, 1.f));
    }

    Ptr<CMaterial> pMtrl = MeshRender()->CreateDynamicMaterial();

    pMtrl->SetScalarParam(INT_0, 1);
    pMtrl->SetScalarParam(FLOAT_0, 0.7f);
    pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 0.f, 0.f, 1.f));

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Appear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Attack.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Hit01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Hit02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Run.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Surprised.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Uturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost\\Ghost_Waiting.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();
}

void CGhostScript::tick()
{
    CEnemyScript::tick();

    // FSM
    switch (m_State)
    {
    case GHOST_STATE::Waiting:
        Waiting();
        break;
    case GHOST_STATE::Appear:
        Appear();
        break;
    case GHOST_STATE::Idle:
        Idle();
        break;
    case GHOST_STATE::Run:
        Run();
        break;
    case GHOST_STATE::Trace:
        Trace();
        break;
    case GHOST_STATE::Attack:
        Attack();
        break;
    case GHOST_STATE::Uturn:
        Uturn();
        break;
    case GHOST_STATE::Hit1:
        Hit1();
        break;
    case GHOST_STATE::Hit2:
        Hit2();
        break;
    case GHOST_STATE::Death:
        Death();
        break;
    }
}

bool CGhostScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (GHOST_STATE::Death == m_State || GHOST_STATE::Waiting == m_State || GHOST_STATE::Appear == m_State)
        return false;

    m_CurLife -= _DamageAmount;

    if (m_CurLife <= 0)
        ChangeState(GHOST_STATE::Death);
    else
    {
        if (m_State != GHOST_STATE::Attack)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (_DamageAmount >= 12.f)
                ChangeState(GHOST_STATE::Hit2);
            else
                ChangeState(GHOST_STATE::Hit1);
        }
    }

    return true;
}

void CGhostScript::ChangeState(GHOST_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CGhostScript::EnterState()
{
    switch (m_State)
    {
    case GHOST_STATE::Waiting: {
        Animator2D()->Play(L"Ghost_Waiting", true);
    }
    break;
    case GHOST_STATE::Appear: {
        Animator2D()->Play(L"Ghost_Appear", false);
    }
    break;
    case GHOST_STATE::Idle: {
        Animator2D()->Play(L"Ghost_Idle", true);
    }
    break;
    case GHOST_STATE::Run: {
        Animator2D()->Play(L"Ghost_Run", true);
    }
    break;
    case GHOST_STATE::Trace: {
        Animator2D()->Play(L"Ghost_Run", true);
    }
    break;
    case GHOST_STATE::Attack: {
        SpawnExclamationMark();
        Animator2D()->Play(L"Ghost_Attack", false);
    }
    break;
    case GHOST_STATE::Uturn: {
        Animator2D()->Play(L"Ghost_Uturn", false);
    }
    break;
    case GHOST_STATE::Hit1: {
        Animator2D()->Play(L"Ghost_Hit01", false);
    }
    break;
    case GHOST_STATE::Hit2: {
        Animator2D()->Play(L"Ghost_Hit02", false);
    }
    break;
    case GHOST_STATE::Death: {
        SpawnFXHealDeflagration();
        SpawnFXGhost();
        Animator2D()->Play(L"Ghost_Death", false);
    }
    break;
    }
}

void CGhostScript::ExitState()
{
    switch (m_State)
    {
    case GHOST_STATE::Waiting: {
    }
    break;
    case GHOST_STATE::Appear: {
    }
    break;
    case GHOST_STATE::Idle: {
    }
    break;
    case GHOST_STATE::Run: {
    }
    break;
    case GHOST_STATE::Trace: {
    }
    break;
    case GHOST_STATE::Attack: {
        SetHitBox(false);
    }
    break;
    case GHOST_STATE::Uturn: {
        RotateTransform();
    }
    break;
    case GHOST_STATE::Hit1: {
    }
    break;
    case GHOST_STATE::Hit2: {
    }
    break;
    case GHOST_STATE::Death: {
    }
    break;
    }
}

void CGhostScript::Waiting()
{
    StopWalking();

    if (nullptr != m_pTarget)
        ChangeState(GHOST_STATE::Appear);
}

void CGhostScript::Appear()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(GHOST_STATE::Idle);
}

void CGhostScript::Idle()
{
    if (nullptr != m_pTarget)
    {
        ChangeState(GHOST_STATE::Trace);
        m_PassedTime = 0.f;
    }

    StopWalking();

    m_PassedTime += DT;

    if (m_PassedTime > 1.f)
    {
        // 방향 전환
        ChangeState(GHOST_STATE::Uturn);
        if (DIRECTION_TYPE::LEFT == m_Dir)
            m_Dir = DIRECTION_TYPE::RIGHT;
        else if (DIRECTION_TYPE::RIGHT == m_Dir)
            m_Dir = DIRECTION_TYPE::LEFT;

        m_PassedTime = 0.f;
    }
}

void CGhostScript::Run()
{
    if (nullptr != m_pTarget)
    {
        ChangeState(GHOST_STATE::Trace);
        m_PassedTime = 0.f;
    }

    Walking();

    m_PassedTime += DT;

    if (m_PassedTime > m_PatrolDuration)
    {
        ChangeState(GHOST_STATE::Idle);
        m_PassedTime = 0.f;
    }
}

void CGhostScript::Trace()
{
    if (nullptr == m_pTarget)
    {
        ChangeState(GHOST_STATE::Idle);
        return;
    }

    Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
    Vec3 pos = Transform()->GetWorldPos();

    TargetPos.z = 0.f;
    pos.z = 0.f;

    Vec3 Dist = TargetPos - pos;

    // 너무 가까운 경우 이동 X
    if (Dist.Length() > 10.f)
    {
        Walking();
    }

    if (Dist.Length() < m_AttackRange)
    {
        ChangeState(GHOST_STATE::Attack);
    }

    // 방향 전환
    if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
    {
        ChangeState(GHOST_STATE::Uturn);
        m_Dir = DIRECTION_TYPE::RIGHT;
    }
    else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
    {
        ChangeState(GHOST_STATE::Uturn);
        m_Dir = DIRECTION_TYPE::LEFT;
    }
}

void CGhostScript::Attack()
{
    static bool HasAttack = false;

    StopWalking();

    if (Animator2D()->IsFinish())
    {
        ChangeState(GHOST_STATE::Idle);
        HasAttack = false;
    }

    if (!HasAttack && 16 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true);

        const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
        for (size_t i = 0; i < vecChild.size(); i++)
        {
            if (L"Ghost_Attack_FX" == vecChild[i]->GetName())
            {
                vecChild[i]->Animator2D()->Play(L"Ghost_Attack_FX", false);
            }
        }

        HasAttack = true;
    }
}

void CGhostScript::Uturn()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(GHOST_STATE::Run);
}

void CGhostScript::Hit1()
{
    if (Animator2D()->IsFinish())
        ChangeState(GHOST_STATE::Idle);
}

void CGhostScript::Hit2()
{
    if (Animator2D()->IsFinish())
        ChangeState(GHOST_STATE::Idle);
}

void CGhostScript::Death()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CGhostScript::OnDetectTargetEnter(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetEnter(_TargetObj);
}

void CGhostScript::OnDetectTargetExit(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetExit(_TargetObj);
}

void CGhostScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
    fwrite(&m_PatrolDuration, sizeof(float), 1, _File);
}

void CGhostScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
    fread(&m_PatrolDuration, sizeof(float), 1, _File);
}
