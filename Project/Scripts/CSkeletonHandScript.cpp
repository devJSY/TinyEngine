#include "pch.h"
#include "CSkeletonHandScript.h"

CSkeletonHandScript::CSkeletonHandScript()
    : CEnemyScript(SKELETONHANDSCRIPT)
    , m_State(SKELETONHAND_STATE::Hide)
{
    m_CurLife = m_MaxLife = 10;
    m_Speed = 0.f;
    m_ATK = 5;
    m_AttackRange = 100.f;
}

CSkeletonHandScript::CSkeletonHandScript(const CSkeletonHandScript& origin)
    : CEnemyScript(origin)
    , m_State(origin.m_State)
{
}

CSkeletonHandScript::~CSkeletonHandScript()
{
}

void CSkeletonHandScript::begin()
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
    pMtrl->SetScalarParam(FLOAT_0, 0.5f);
    pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 0.f, 0.f, 1.f));

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\GPE_SkeletonHand\\GPE_SkeletonHand_Appear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\GPE_SkeletonHand\\GPE_SkeletonHand_Attack.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\GPE_SkeletonHand\\GPE_SkeletonHand_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Enemy\\GPE_SkeletonHand\\GPE_SkeletonHand_Idle.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    RotateTransform();
}

void CSkeletonHandScript::tick()
{
    CEnemyScript::tick();

    // FSM
    switch (m_State)
    {
    case SKELETONHAND_STATE::Hide:
        Hide();
        break;
    case SKELETONHAND_STATE::Appear:
        Appear();
        break;
    case SKELETONHAND_STATE::Idle:
        Idle();
        break;
    case SKELETONHAND_STATE::Attack:
        Attack();
        break;
    case SKELETONHAND_STATE::Death:
        Death();
        break;
    }
}

bool CSkeletonHandScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (SKELETONHAND_STATE::Death == m_State || SKELETONHAND_STATE::Hide == m_State || SKELETONHAND_STATE::Appear == m_State)
        return false;

    m_CurLife -= _DamageAmount;

    if (m_CurLife <= 0)
        ChangeState(SKELETONHAND_STATE::Death);

    return true;
}

void CSkeletonHandScript::ChangeState(SKELETONHAND_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CSkeletonHandScript::EnterState()
{
    switch (m_State)
    {
    case SKELETONHAND_STATE::Hide: {
    }
    break;
    case SKELETONHAND_STATE::Appear: {
        Animator2D()->Play(L"GPE_SkeletonHand_Appear", true);
    }
    break;
    case SKELETONHAND_STATE::Idle: {
        Animator2D()->Play(L"GPE_SkeletonHand_Idle", true);
    }
    break;
    case SKELETONHAND_STATE::Attack: {
        Animator2D()->Play(L"GPE_SkeletonHand_Attack", false);
    }
    break;
    case SKELETONHAND_STATE::Death: {
        Animator2D()->Play(L"GPE_SkeletonHand_Death", false);
    }
    break;
    }
}

void CSkeletonHandScript::ExitState()
{
    switch (m_State)
    {
    case SKELETONHAND_STATE::Hide: {
    }
    break;
    case SKELETONHAND_STATE::Appear: {
    }
    break;
    case SKELETONHAND_STATE::Idle: {
    }
    break;
    case SKELETONHAND_STATE::Attack: {
        SetHitBox(false);
    }
    break;
    case SKELETONHAND_STATE::Death: {
    }
    break;
    }
}

void CSkeletonHandScript::Hide()
{
    if (nullptr != m_pTarget)
        ChangeState(SKELETONHAND_STATE::Appear);
}

void CSkeletonHandScript::Appear()
{
    if (Animator2D()->IsFinish())
        ChangeState(SKELETONHAND_STATE::Idle);
}

void CSkeletonHandScript::Idle()
{
    if (nullptr == m_pTarget)
        return;

    Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
    Vec3 pos = Transform()->GetWorldPos();

    TargetPos.z = 0.f;
    pos.z = 0.f;

    Vec3 Dist = TargetPos - pos;

    if (Dist.Length() < m_AttackRange)
    {
        ChangeState(SKELETONHAND_STATE::Attack);
    }
}

void CSkeletonHandScript::Attack()
{
    static bool HasAttack = false;

    if (Animator2D()->IsFinish())
    {
        ChangeState(SKELETONHAND_STATE::Idle);
        HasAttack = false;
    }

    if (!HasAttack && 23 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true);
        HasAttack = true;
    }
}

void CSkeletonHandScript::Death()
{
    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CSkeletonHandScript::OnDetectTargetEnter(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetEnter(_TargetObj);
}

void CSkeletonHandScript::OnDetectTargetExit(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetExit(_TargetObj);
}

void CSkeletonHandScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
}

void CSkeletonHandScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
}
