#include "pch.h"
#include "CFlyingBookScript.h"
#include "CProjectile2DScript.h"

CFlyingBookScript::CFlyingBookScript()
    : CEnemyScript(FLYINGBOOKSCRIPT)
    , m_State(FLYINGBOOK_STATE::Idle)
    , m_PassedTime(0.f)
    , m_PatrolDuration(1.5f)
{
    m_CurLife = m_MaxLife = 15;
    m_Speed = 3;
    m_ATK = 5;
    m_AttackRange = 200.f;

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PatrolDuration, "Patrol Duration");
}

CFlyingBookScript::CFlyingBookScript(const CFlyingBookScript& origin)
    : CEnemyScript(origin)
    , m_State(origin.m_State)
    , m_PassedTime(0.f)
    , m_PatrolDuration(origin.m_PatrolDuration)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PatrolDuration, "Patrol Duration");
}

CFlyingBookScript::~CFlyingBookScript()
{
}

void CFlyingBookScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DGlowMtrl"));

        Transform()->SetRelativeScale(Vec3(512.f, 512.f, 1.f));
    }

    Ptr<CMaterial> pMtrl = MeshRender()->CreateDynamicMaterial();

    pMtrl->SetScalarParam(INT_0, 1);
    pMtrl->SetScalarParam(FLOAT_0, 0.9f);
    pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 0.f, 0.f, 1.f));

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\W1_Enemy_PossessedBook\\W1_Enemy_PossessedBook_Attack.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\W1_Enemy_PossessedBook\\W1_Enemy_PossessedBook_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\W1_Enemy_PossessedBook\\W1_Enemy_PossessedBook_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\W1_Enemy_PossessedBook\\W1_Enemy_PossessedBook_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\W1_Enemy_PossessedBook\\W1_Enemy_PossessedBook_Uturn.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();
}

void CFlyingBookScript::tick()
{
    CEnemyScript::tick();

    // FSM
    switch (m_State)
    {
    case FLYINGBOOK_STATE::Idle:
        Idle();
        break;
    case FLYINGBOOK_STATE::Attack:
        Attack();
        break;
    case FLYINGBOOK_STATE::Uturn:
        Uturn();
        break;
    case FLYINGBOOK_STATE::Hit:
        Hit();
        break;
    case FLYINGBOOK_STATE::Death:
        Death();
        break;
    }
}

bool CFlyingBookScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (FLYINGBOOK_STATE::Death == m_State)
        return false;

    m_CurLife -= _DamageAmount;

    if (m_CurLife <= 0)
        ChangeState(FLYINGBOOK_STATE::Death);
    else
    {
        if (m_State != FLYINGBOOK_STATE::Attack)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            ChangeState(FLYINGBOOK_STATE::Hit);
        }
    }

    return true;
}

void CFlyingBookScript::ChangeState(FLYINGBOOK_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CFlyingBookScript::EnterState()
{
    switch (m_State)
    {
    case FLYINGBOOK_STATE::Idle: {
        Animator2D()->Play(L"W1_Enemy_PossessedBook_Idle", true);
    }
    break;
    case FLYINGBOOK_STATE::Attack: {
        Animator2D()->Play(L"W1_Enemy_PossessedBook_Attack", false);
    }
    break;
    case FLYINGBOOK_STATE::Uturn: {
        Animator2D()->Play(L"W1_Enemy_PossessedBook_Uturn", false);
    }
    break;
    case FLYINGBOOK_STATE::Hit: {
        Animator2D()->Play(L"W1_Enemy_PossessedBook_Hit", false);
    }
    break;
    case FLYINGBOOK_STATE::Death: {
        Animator2D()->Play(L"W1_Enemy_PossessedBook_Death", false);
    }
    break;
    }
}

void CFlyingBookScript::ExitState()
{
    switch (m_State)
    {
    case FLYINGBOOK_STATE::Idle:
        break;
    case FLYINGBOOK_STATE::Attack:
        break;
    case FLYINGBOOK_STATE::Uturn:
        RotateTransform();
        break;
    case FLYINGBOOK_STATE::Hit:
        break;
    case FLYINGBOOK_STATE::Death:
        break;
    }
}

void CFlyingBookScript::Idle()
{
    if (nullptr != m_pTarget)
    {
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
            ChangeState(FLYINGBOOK_STATE::Attack);
            m_PassedTime = 0.f;
        }
    }
    else
    {
        Walking();
    }

    m_PassedTime += DT;

    if (m_PassedTime > m_PatrolDuration)
    {
        // 방향 전환
        ChangeState(FLYINGBOOK_STATE::Uturn);
        if (DIRECTION_TYPE::LEFT == m_Dir)
            m_Dir = DIRECTION_TYPE::RIGHT;
        else if (DIRECTION_TYPE::RIGHT == m_Dir)
            m_Dir = DIRECTION_TYPE::LEFT;

        m_PassedTime = 0.f;
    }
}

void CFlyingBookScript::Attack()
{
    static bool HasAttack = false;

    StopWalking();

    if (Animator2D()->IsFinish())
    {
        ChangeState(FLYINGBOOK_STATE::Idle);
        HasAttack = false;
    }

    if (nullptr != m_pTarget && !HasAttack && 24 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        Ptr<CPrefab> pProjectilePref =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FlyingBook_Projectile.pref", L"prefab\\FlyingBook_Projectile.pref");
        CGameObject* pProjectile = pProjectilePref->Instantiate();
        pProjectile->Transform()->SetRelativePos(Transform()->GetRelativePos());

        CProjectile2DScript* ProjScript = pProjectile->GetScript<CProjectile2DScript>();
        ProjScript->SetTarget(m_pTarget, Vec3(1.f, 0.f, 0.f));

        GamePlayStatic::SpawnGameObject(pProjectile, 0);

        HasAttack = true;
    }
}

void CFlyingBookScript::Uturn()
{
    StopMoving();

    if (Animator2D()->IsFinish())
        ChangeState(FLYINGBOOK_STATE::Idle);
}

void CFlyingBookScript::Hit()
{
    if (Animator2D()->IsFinish())
        ChangeState(FLYINGBOOK_STATE::Idle);
}

void CFlyingBookScript::Death()
{
    StopMoving();

    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CFlyingBookScript::OnDetectTargetEnter(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetEnter(_TargetObj);
}

void CFlyingBookScript::OnDetectTargetExit(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetExit(_TargetObj);
}

void CFlyingBookScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
    fwrite(&m_PatrolDuration, sizeof(float), 1, _File);
}

void CFlyingBookScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
    fread(&m_PatrolDuration, sizeof(float), 1, _File);
}
