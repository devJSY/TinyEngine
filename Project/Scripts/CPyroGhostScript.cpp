#include "pch.h"
#include "CPyroGhostScript.h"

#include "CPlayerCameraScript.h"
#include <Engine\\components.h>

CPyroGhostScript::CPyroGhostScript()
    : CEnemyScript(PYROGHOSTSCRIPT)
    , m_State(PYROGHOST_STATE::Hide)
    , m_PassedTime(0.f)
    , m_PatrolDuration(2.f)
{
    m_Life = 50;
    m_Speed = 5;
    m_ATK = 7;
    m_AttackRange = 200.f;

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PatrolDuration, "Patrol Duration");
}

CPyroGhostScript::CPyroGhostScript(const CPyroGhostScript& origin)
    : CEnemyScript(origin)
    , m_State(origin.m_State)
    , m_PassedTime(0.f)
    , m_PatrolDuration(origin.m_PatrolDuration)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PatrolDuration, "Patrol Duration");
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
    CEnemyScript::tick();

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

bool CPyroGhostScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (PYROGHOST_STATE::Death == m_State || PYROGHOST_STATE::Hide == m_State || PYROGHOST_STATE::Appear == m_State)
        return false;

    m_Life -= _DamageAmount;

    if (m_Life <= 0)
        ChangeState(PYROGHOST_STATE::Death);
    else
    {
        if (m_State != PYROGHOST_STATE::Attack)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (m_Life < 20.f)
                ChangeState(PYROGHOST_STATE::Hit2);
            else
                ChangeState(PYROGHOST_STATE::Hit1);
        }
    }

    return true;
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
        SetHitBox(false);
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
    if (nullptr != m_pTarget)
        ChangeState(PYROGHOST_STATE::Trace);

    Walking();

    m_PassedTime += DT;

    if (m_PassedTime > m_PatrolDuration)
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

    if (nullptr != m_pTarget)
        ChangeState(PYROGHOST_STATE::Appear);
}

void CPyroGhostScript::Appear()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::Trace()
{
    if (nullptr == m_pTarget)
    {
        ChangeState(PYROGHOST_STATE::Idle);
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
    static bool HasAttack = false;

    StopWalking();

    if (Animator2D()->IsFinish())
    {
        ChangeState(PYROGHOST_STATE::Idle);
        HasAttack = false;
    }

    if (!HasAttack && 8 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
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

void CPyroGhostScript::Uturn()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::Spotted()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::Hit1()
{
    if (Animator2D()->IsFinish())
        ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::Hit2()
{
    if (Animator2D()->IsFinish())
        ChangeState(PYROGHOST_STATE::Idle);
}

void CPyroGhostScript::Death()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CPyroGhostScript::OnDetectTargetEnter(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetEnter(_TargetObj);
}

void CPyroGhostScript::OnDetectTargetExit(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetExit(_TargetObj);
}

void CPyroGhostScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
    fwrite(&m_PatrolDuration, sizeof(float), 1, _File);
}

void CPyroGhostScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
    fread(&m_PatrolDuration, sizeof(float), 1, _File);
}
