#include "pch.h"
#include "CLifeScript.h"
#include "CPlayerCameraScript.h"

CLifeScript::CLifeScript()
    : CBossEnemyScript(LIFESCRIPT)
    , m_State(LIFE_STATE::Hide)
    , m_PassedTime(0.f)
    , m_bAttackStart(false)
    , m_bAttackEnd(false)
{
    m_CurLife = m_MaxLife = 2000;
    m_Speed = 10;
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
    pMtrl->SetScalarParam(VEC4_0, Vec4(0.2f, 0.2f, 0.f, 1.f));

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
    case LIFE_STATE::SecondPhase:
        SecondPhase();
        break;
    case LIFE_STATE::ThirdPhase:
        ThirdPhase();
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
    case LIFE_STATE::Attack1:
        Attack1();
        break;
    case LIFE_STATE::Attack2:
        Attack2();
        break;
    case LIFE_STATE::Attack3:
        Attack3();
        break;
    case LIFE_STATE::Attack4:
        Attack4();
        break;
    case LIFE_STATE::Attack5:
        Attack5();
        break;
    case LIFE_STATE::Skill1:
        Skill1();
        break;
    case LIFE_STATE::Skill2:
        Skill2();
        break;
    case LIFE_STATE::Death:
        Death();
        break;
    }
}

bool CLifeScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (LIFE_STATE::Death == m_State || LIFE_STATE::Intro == m_State)
        return false;

    m_CurLife -= _DamageAmount;

    if (m_CurLife <= 0)
        ChangeState(LIFE_STATE::Death);
    else
    {
        if (LIFE_STATE::Attack1 != m_State && LIFE_STATE::Attack2 != m_State && LIFE_STATE::Attack3 != m_State && LIFE_STATE::Attack4 != m_State &&
            LIFE_STATE::Attack5 != m_State && LIFE_STATE::Skill1 != m_State && LIFE_STATE::Skill2 != m_State && LIFE_STATE::Stun != m_State)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (_DamageAmount >= 15.f)
                ChangeState(LIFE_STATE::Stun);
            else
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
        Animator2D()->Play(L"W09_Boss_NatalieT_Idle", true);
    }
    break;
    case LIFE_STATE::Intro: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Intro", false);

        // UI 설정
        SpawnBossUI(BOSS_TYPE::LIFE);
    }
    break;
    case LIFE_STATE::SecondPhase: {
    }
    break;
    case LIFE_STATE::ThirdPhase: {
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
    case LIFE_STATE::Attack1: {
        StopMoving();
        m_bAttackStart = false;
        m_bAttackEnd = false;
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack01", false);
    }
    break;
    case LIFE_STATE::Attack2: {
        StopMoving();
        m_bAttackStart = false;
        m_bAttackEnd = false;
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack02", false);
    }
    break;
    case LIFE_STATE::Attack3: {
        StopMoving();
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack03", false);
    }
    break;
    case LIFE_STATE::Attack4: {
        StopMoving();
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack05", false);
    }
    break;
    case LIFE_STATE::Attack5: {
        StopMoving();
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack08", false);
    }
    break;
    case LIFE_STATE::Skill1: {
        StopMoving();
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack06", false);
    }
    break;
    case LIFE_STATE::Skill2: {
        StopMoving();
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack07", false);
    }
    break;
    case LIFE_STATE::Death: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Death", false);

        // BossUI 삭제
        DestroyBossUI();
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
    case LIFE_STATE::SecondPhase: {
    }
    break;
    case LIFE_STATE::ThirdPhase: {
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
    case LIFE_STATE::Attack1: {
        SetHitBox(false);
    }
    break;
    case LIFE_STATE::Attack2: {
        StopMoving();
        Rigidbody2D()->SetGravityScale(1.f);
        SetHitBox(false);
    }
    break;
    case LIFE_STATE::Attack3: {
        SetHitBox(false);
    }
    break;
    case LIFE_STATE::Attack4: {
    }
    break;
    case LIFE_STATE::Attack5: {
    }
    break;
    case LIFE_STATE::Skill1: {
    }
    break;
    case LIFE_STATE::Skill2: {
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

void CLifeScript::SecondPhase()
{
}

void CLifeScript::ThirdPhase()
{
}

void CLifeScript::Idle()
{
    StopWalking();

    m_PassedTime += DT;

    Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
    Vec3 pos = Transform()->GetWorldPos();
    TargetPos.z = 0.f;
    pos.z = 0.f;
    Vec3 Dist = TargetPos - pos;

    // 공격 범위 이내에 존재한다면 공격
    if (Dist.Length() < m_AttackRange)
    {
        // int AttackState = GetRandomInt(1, 5);
        // 테스트
        int AttackState = GetRandomInt(2, 2);

        if (1 == AttackState)
            ChangeState(LIFE_STATE::Attack1);
        else if (2 == AttackState)
            ChangeState(LIFE_STATE::Attack2);
        else if (3 == AttackState)
            ChangeState(LIFE_STATE::Attack3);
        else if (4 == AttackState)
            ChangeState(LIFE_STATE::Attack4);
        else if (5 == AttackState)
            ChangeState(LIFE_STATE::Attack5);

        // 방향 전환
        if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
        {
            m_Dir = DIRECTION_TYPE::RIGHT;
            RotateTransform();
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
        {
            m_Dir = DIRECTION_TYPE::LEFT;
            RotateTransform();
        }

        m_PassedTime = 0.f;
    }

    if (m_PassedTime > 0.3f)
    {
        ChangeState(LIFE_STATE::Run);

        // 방향 전환
        if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
        {
            m_Dir = DIRECTION_TYPE::RIGHT;
            RotateTransform();
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
        {
            m_Dir = DIRECTION_TYPE::LEFT;
            RotateTransform();
        }

        m_PassedTime = 0.f;
    }
}

void CLifeScript::Run()
{
    Walking();

    m_PassedTime += DT;

    Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
    Vec3 pos = Transform()->GetWorldPos();
    TargetPos.z = 0.f;
    pos.z = 0.f;
    Vec3 Dist = TargetPos - pos;

    // 공격 범위 이내에 존재한다면 공격
    if (Dist.Length() < m_AttackRange)
    {
        // int AttackState = GetRandomInt(1, 5);
        // 테스트
        int AttackState = GetRandomInt(2, 2);

        if (1 == AttackState)
            ChangeState(LIFE_STATE::Attack1);
        else if (2 == AttackState)
            ChangeState(LIFE_STATE::Attack2);
        else if (3 == AttackState)
            ChangeState(LIFE_STATE::Attack3);
        else if (4 == AttackState)
            ChangeState(LIFE_STATE::Attack4);
        else if (5 == AttackState)
            ChangeState(LIFE_STATE::Attack5);

        // 방향 전환
        if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
        {
            m_Dir = DIRECTION_TYPE::RIGHT;
            RotateTransform();
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
        {
            m_Dir = DIRECTION_TYPE::LEFT;
            RotateTransform();
        }

        m_PassedTime = 0.f;
    }

    if (m_PassedTime > 1.f)
    {
        ChangeState(LIFE_STATE::Idle);
        m_PassedTime = 0.f;
    }
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

void CLifeScript::Attack1()
{
    if (Animator2D()->IsFinish())
    {
        ChangeState(LIFE_STATE::Idle);
    }

    if (!m_bAttackStart && 11 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true, L"Attack1_HitBox");
        m_bAttackStart = true;

        Vec2 Dir = Vec2();

        if (DIRECTION_TYPE::RIGHT == m_Dir)
            Dir = Vec2(1.f, 0.f);
        else
            Dir = Vec2(-1.f, 0.f);

        Rigidbody2D()->AddForce(Dir * GetRandomfloat(20.f, 40.f), ForceMode2D::Impulse);
    }
    else if (!m_bAttackEnd && 19 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(false, L"Attack1_HitBox");
        m_bAttackEnd = true;
    }

    if (m_bAttackStart && 15 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        StopMoving();
    }
}

void CLifeScript::Attack2()
{
    static bool bShaked = false;

    if (Animator2D()->IsFinish())
    {
        ChangeState(LIFE_STATE::Idle);
        bShaked = false;
    }

    int CurAnimFrm = Animator2D()->GetCurAnim()->GetCurFrmIdx();

    if ((0 <= CurAnimFrm && 25 >= CurAnimFrm) || (52 <= CurAnimFrm && 58 >= CurAnimFrm))
    {
        Rigidbody2D()->SetGravityScale(1.f);
        if (0 <= CurAnimFrm && 25 >= CurAnimFrm)
            Rigidbody2D()->SetVelocity(Vec2(0.f, 3.f));
        else
            Rigidbody2D()->SetVelocity(Vec2(0.f, 8.f));

        m_bAttackStart = false;
        m_bAttackEnd = false;
        bShaked = false;
    }
    else if (26 == CurAnimFrm || 59 == CurAnimFrm)
    {
        StopMoving();
    }
    else if ((27 <= CurAnimFrm && 36 >= CurAnimFrm) || (60 <= CurAnimFrm && 70 >= CurAnimFrm))
    {
        Rigidbody2D()->SetGravityScale(0.f);
        Walking();
    }
    else if (!m_bAttackStart && 37 == CurAnimFrm || !m_bAttackStart && 71 == CurAnimFrm)
    {
        StopWalking();
        Rigidbody2D()->SetGravityScale(3.f);
        SetHitBox(true, L"Attack2_HitBox");
        m_bAttackStart = true;

        Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
        Vec3 pos = Transform()->GetWorldPos();
        TargetPos.z = 0.f;
        pos.z = 0.f;
        Vec3 Dist = TargetPos - pos;

        // 방향 전환
        if (DIRECTION_TYPE::LEFT == m_Dir && Dist.x > 0.f)
        {
            m_Dir = DIRECTION_TYPE::RIGHT;
            RotateTransform();
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir && Dist.x < 0.f)
        {
            m_Dir = DIRECTION_TYPE::LEFT;
            RotateTransform();
        }
    }
    else if (!bShaked && 46 == CurAnimFrm || !bShaked && 80 == CurAnimFrm)
    {
        // Camera Shake
        CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
        if (nullptr != pPlayerCamObj)
        {
            CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
            if (nullptr != pScript)
            {
                pScript->ShakeCam(ShakeDir::Vertical, 0.2f, 5.f);
            }
        }

        bShaked = true;
    }
    else if (!m_bAttackEnd && 51 == CurAnimFrm || !m_bAttackEnd && 93 == CurAnimFrm)
    {
        StopMoving();
        Rigidbody2D()->SetGravityScale(1.f);
        SetHitBox(false, L"Attack2_HitBox");
        m_bAttackEnd = true;
    }
}
void CLifeScript::Attack3()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Attack4()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Attack5()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Skill1()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Skill2()
{
    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Death()
{
    StopWalking();

    if (Animator2D()->IsFinish())
    {
        EndBossBattle();

        GamePlayStatic::DestroyGameObject(GetOwner());
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
