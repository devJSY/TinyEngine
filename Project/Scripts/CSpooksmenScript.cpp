#include "pch.h"
#include "CSpooksmenScript.h"

CSpooksmenScript::CSpooksmenScript()
    : CEnemyScript(SPOOKSMENSCRIPT)
    , m_State(SPOOKSMEN_STATE::Hide)
    , m_AttackCount(0)
{
    m_Life = 200;
    m_Speed = 3;
    m_ATK = 10;
    m_AttackRange = 200.f;
}

CSpooksmenScript::CSpooksmenScript(const CSpooksmenScript& origin)
    : CEnemyScript(origin)
    , m_State(origin.m_State)
    , m_AttackCount(0)
{
}

CSpooksmenScript::~CSpooksmenScript()
{
}

void CSpooksmenScript::begin()
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
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Appear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Attack01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Attack02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Attack03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Attack04.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Hit01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Hit02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Surprised.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\Ghost_Med\\Ghost_Med_Uturn.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();
}

void CSpooksmenScript::tick()
{
    CEnemyScript::tick();

    // FSM
    switch (m_State)
    {
    case SPOOKSMEN_STATE::Hide:
        Hide();
        break;
    case SPOOKSMEN_STATE::Appear:
        Appear();
        break;
    case SPOOKSMEN_STATE::Idle:
        Idle();
        break;
    case SPOOKSMEN_STATE::Trace:
        Trace();
        break;
    case SPOOKSMEN_STATE::Attack:
        Attack();
        break;
    case SPOOKSMEN_STATE::Uturn:
        Uturn();
        break;
    case SPOOKSMEN_STATE::Hit1:
        Hit1();
        break;
    case SPOOKSMEN_STATE::Hit2:
        Hit2();
        break;
    case SPOOKSMEN_STATE::Surprised:
        Surprised();
        break;
    case SPOOKSMEN_STATE::Death:
        Death();
        break;
    }
}

void CSpooksmenScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (SPOOKSMEN_STATE::Death == m_State || SPOOKSMEN_STATE::Appear == m_State)
        return;

    m_Life -= _DamageAmount;

    if (m_Life <= 0)
        ChangeState(SPOOKSMEN_STATE::Death);
    else
    {
        if (m_State != SPOOKSMEN_STATE::Attack)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (m_Life < 20.f)
                ChangeState(SPOOKSMEN_STATE::Hit2);
            else
                ChangeState(SPOOKSMEN_STATE::Hit1);
        }
    }
}

void CSpooksmenScript::ChangeState(SPOOKSMEN_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CSpooksmenScript::EnterState()
{
    switch (m_State)
    {
    case SPOOKSMEN_STATE::Hide: {
    }
    break;
    case SPOOKSMEN_STATE::Appear: {
        Animator2D()->Play(L"Ghost_Med_Appear", false);
    }
    break;
    case SPOOKSMEN_STATE::Idle: {
        Animator2D()->Play(L"Ghost_Med_Idle", true);
    }
    break;
    case SPOOKSMEN_STATE::Trace: {
        Animator2D()->Play(L"Ghost_Med_Idle", true);
    }
    break;
    case SPOOKSMEN_STATE::Attack: {
        if (1 == m_AttackCount)
            Animator2D()->Play(L"Ghost_Med_Attack01", false);
        else if (2 == m_AttackCount)
            Animator2D()->Play(L"Ghost_Med_Attack02", false);
        else if (3 == m_AttackCount)
            Animator2D()->Play(L"Ghost_Med_Attack03", false);
        else if (4 == m_AttackCount)
            Animator2D()->Play(L"Ghost_Med_Attack04", false);
    }
    break;
    case SPOOKSMEN_STATE::Uturn: {
        Animator2D()->Play(L"Ghost_Med_Uturn", false);
    }
    break;
    case SPOOKSMEN_STATE::Hit1: {
        Animator2D()->Play(L"Ghost_Med_Hit01", false);
    }
    break;
    case SPOOKSMEN_STATE::Hit2: {
        Animator2D()->Play(L"Ghost_Med_Hit02", false);
    }
    break;
    case SPOOKSMEN_STATE::Surprised: {
        Animator2D()->Play(L"Ghost_Med_Surprised", false);
    }
    break;
    case SPOOKSMEN_STATE::Death: {
        Animator2D()->Play(L"Ghost_Med_Death", false);
    }
    break;
    }
}

void CSpooksmenScript::ExitState()
{
    switch (m_State)
    {
    case SPOOKSMEN_STATE::Hide: {
    }
    break;
    case SPOOKSMEN_STATE::Appear: {
    }
    break;
    case SPOOKSMEN_STATE::Idle: {
    }
    break;
    case SPOOKSMEN_STATE::Trace: {
    }
    break;
    case SPOOKSMEN_STATE::Attack: {
        SetHitBox(false);
    }
    break;
    case SPOOKSMEN_STATE::Uturn: {
        RotateTransform();
    }
    break;
    case SPOOKSMEN_STATE::Hit1: {
    }
    break;
    case SPOOKSMEN_STATE::Hit2: {
    }
    break;
    case SPOOKSMEN_STATE::Surprised: {
    }
    break;
    case SPOOKSMEN_STATE::Death: {
    }
    break;
    }
}

void CSpooksmenScript::Hide()
{
    StopWalking();

    if (nullptr != m_pTarget)
        ChangeState(SPOOKSMEN_STATE::Appear);
}

void CSpooksmenScript::Appear()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(SPOOKSMEN_STATE::Idle);
}

void CSpooksmenScript::Idle()
{
    if (nullptr != m_pTarget)
    {
        ChangeState(SPOOKSMEN_STATE::Trace);
    }

    StopWalking();
}

void CSpooksmenScript::Trace()
{
    if (nullptr == m_pTarget)
    {
        ChangeState(SPOOKSMEN_STATE::Idle);
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
        m_AttackCount = GetRandomInt(1, 4);
        ChangeState(SPOOKSMEN_STATE::Attack);
    }

    // 방향 전환
    if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
    {
        ChangeState(SPOOKSMEN_STATE::Uturn);
        m_Dir = DIRECTION_TYPE::RIGHT;
    }
    else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
    {
        ChangeState(SPOOKSMEN_STATE::Uturn);
        m_Dir = DIRECTION_TYPE::LEFT;
    }
}

void CSpooksmenScript::Attack()
{
    static bool HasAttack = false;

    StopWalking();

    if (Animator2D()->IsFinish())
    {
        ChangeState(SPOOKSMEN_STATE::Idle);
        HasAttack = false;
    }

    if (!HasAttack && 6 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        if (1 == m_AttackCount)
            SetHitBox(true, L"HitBox1");
        else if (2 == m_AttackCount)
            SetHitBox(true, L"HitBox2");
        else if (3 == m_AttackCount)
            SetHitBox(true, L"HitBox2");
        else if (4 == m_AttackCount)
            SetHitBox(true, L"HitBox3");

        HasAttack = true;
    }
}

void CSpooksmenScript::Uturn()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(SPOOKSMEN_STATE::Idle);
}

void CSpooksmenScript::Hit1()
{
    if (Animator2D()->IsFinish())
        ChangeState(SPOOKSMEN_STATE::Idle);
}

void CSpooksmenScript::Hit2()
{
    if (Animator2D()->IsFinish())
        ChangeState(SPOOKSMEN_STATE::Idle);
}

void CSpooksmenScript::Surprised()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        ChangeState(SPOOKSMEN_STATE::Trace);
    }
}

void CSpooksmenScript::Death()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CSpooksmenScript::OnDetectTargetEnter(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetEnter(_TargetObj);

    if (SPOOKSMEN_STATE::Idle == m_State)
        ChangeState(SPOOKSMEN_STATE::Surprised);
}

void CSpooksmenScript::OnDetectTargetExit(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetExit(_TargetObj);
}

void CSpooksmenScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
}

void CSpooksmenScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
}
