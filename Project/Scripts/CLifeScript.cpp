#include "pch.h"
#include "CLifeScript.h"

CLifeScript::CLifeScript()
    : CBossEnemyScript(LIFESCRIPT)
    , m_State(LIFE_STATE::Hide)
    , m_AttackCount(0)
    , m_PassedTime(0.f)
{
    m_CurLife = m_MaxLife = 5000;
    m_Speed = 3;
    m_ATK = 25;
    m_AttackRange = 200.f;
}

CLifeScript::~CLifeScript()
{
}

void CLifeScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DGlowMtrl"));

        Transform()->SetRelativeScale(Vec3(2048.f, 2048.f, 1.f));
    }

    Ptr<CMaterial> pMtrl = MeshRender()->CreateDynamicMaterial();

    pMtrl->SetScalarParam(INT_0, 1);
    pMtrl->SetScalarParam(FLOAT_0, 0.9f);
    pMtrl->SetScalarParam(VEC4_0, Vec4(0.5f, 0.5f, 0.f, 1.f));

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack04.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack05.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack06.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack07.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Attack08.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Hit.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Intro.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Run.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Stun.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Boss\\W09_Boss_NatalieT\\W09_Boss_NatalieT_Uturn.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    m_Dir = DIRECTION_TYPE::LEFT;
    RotateTransform();

    // 타겟 등록
    m_pTarget = CGameManagerScript::GetInset()->GetPlayer();
}

void CLifeScript::tick()
{
    CBossEnemyScript::tick();

    switch (m_State)
    {
    case LIFE_STATE::Hide:
        Hide();
        break;
    case LIFE_STATE::Intro:
        Intro();
        break;
    case LIFE_STATE::Idle:
        Idle();
        break;
    case LIFE_STATE::Run:
        Run();
        break;
    case LIFE_STATE::Hit:
        Hit();
        break;
    case LIFE_STATE::Stun:
        Stun();
        break;
    case LIFE_STATE::Uturn:
        Uturn();
        break;
    case LIFE_STATE::Attack:
        Attack();
        break;
    case LIFE_STATE::Death:
        Death();
        break;
    }
}

bool CLifeScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (LIFE_STATE::Death == m_State || LIFE_STATE::Hide == m_State || LIFE_STATE::Intro == m_State)
        return false;

    m_CurLife -= _DamageAmount;

    if (m_CurLife <= 0)
        ChangeState(LIFE_STATE::Death);
    else
    {
        if (LIFE_STATE::Attack != m_State && LIFE_STATE::Stun != m_State && LIFE_STATE::Hit != m_State)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (_DamageAmount >= 15.f)
                ChangeState(LIFE_STATE::Stun);
            else if (_DamageAmount >= 12.f)
                ChangeState(LIFE_STATE::Hit);
        }
    }

    DamageLifeBarUpdate();

    return true;
}

void CLifeScript::ChangeState(LIFE_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CLifeScript::EnterState()
{
    switch (m_State)
    {
    case LIFE_STATE::Hide: {
    }
    break;
    case LIFE_STATE::Intro: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Intro", false);

        // UI 설정
        SpawnBossUI(BOSS_TYPE::LIFE);
    }
    break;
    case LIFE_STATE::Idle: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Idle", true);
    }
    break;
    case LIFE_STATE::Run: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Run", true);
    }
    break;
    case LIFE_STATE::Hit: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Hit", false);
    }
    break;
    case LIFE_STATE::Stun: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Stun", false);
    }
    break;
    case LIFE_STATE::Uturn: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Uturn", false);
    }
    break;
    case LIFE_STATE::Attack: {
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack01", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack02", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack03", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack04", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack05", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack06", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack07", false);
        // Animator2D()->Play(L"W09_Boss_NatalieT_Attack08", false);
    }
    break;
    case LIFE_STATE::Death: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Death", false);
    }
    break;
    }
}

void CLifeScript::ExitState()
{
    switch (m_State)
    {
    case LIFE_STATE::Hide: {
    }
    break;
    case LIFE_STATE::Intro: {
    }
    break;
    case LIFE_STATE::Idle: {
    }
    break;
    case LIFE_STATE::Run: {
    }
    break;
    case LIFE_STATE::Hit: {
    }
    break;
    case LIFE_STATE::Stun: {
    }
    break;
    case LIFE_STATE::Uturn: {
        RotateTransform();
    }
    break;
    case LIFE_STATE::Attack: {
        SetHitBox(false);
    }
    break;
    case LIFE_STATE::Death: {
    }
    break;
    }
}

void CLifeScript::Hide()
{
}

void CLifeScript::Intro()
{
    StopWalking();

    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Idle()
{
    StopWalking();
}

void CLifeScript::Run()
{
    Walking();
}

void CLifeScript::Hit()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Stun()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Uturn()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Attack()
{
}

void CLifeScript::Death()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        EndBossBattle();

        ChangeState(LIFE_STATE::Hide);
    }
}

void CLifeScript::SaveToLevelFile(FILE* _File)
{
    CBossEnemyScript::SaveToLevelFile(_File);
}

void CLifeScript::LoadFromLevelFile(FILE* _File)
{
    CBossEnemyScript::LoadFromLevelFile(_File);
}
