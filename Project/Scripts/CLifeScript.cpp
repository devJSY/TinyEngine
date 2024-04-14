#include "pch.h"
#include "CLifeScript.h"
#include "CPlayerScript.h"
#include "CPlayerCameraScript.h"
#include "CProjectile2DScript.h"

CLifeScript::CLifeScript()
    : CBossEnemyScript(LIFESCRIPT)
    , m_State(LIFE_STATE::Hide)
    , m_PassedTime(0.f)
    , m_bAttackStart(false)
    , m_bAttackEnd(false)
    , m_CurPhase(1)
    , m_pFeatherProjPref(nullptr)
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
    CBossEnemyScript::begin();

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

    // 프리팹 로딩
    m_pFeatherProjPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Feather_Projectile.pref", L"prefab\\Feather_Projectile.pref");
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

    // 시연용 강제 상태변경 코드
    if (KEY_TAP(KEY::NUM1))
    {
        ChangeState(LIFE_STATE::Attack1);
    }
    else if (KEY_TAP(KEY::NUM2))
    {
        ChangeState(LIFE_STATE::Attack2);
    }
    else if (KEY_TAP(KEY::NUM3))
    {
        ChangeState(LIFE_STATE::Attack3);
    }
    else if (KEY_TAP(KEY::NUM4))
    {
        ChangeState(LIFE_STATE::Attack4);
    }
}

bool CLifeScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (LIFE_STATE::Death == m_State || LIFE_STATE::Intro == m_State || LIFE_STATE::SecondPhase == m_State || LIFE_STATE::ThirdPhase == m_State)
        return false;

    m_CurLife -= _DamageAmount;

    if (m_CurLife <= 0)
        ChangeState(LIFE_STATE::Death);
    else if (1 == m_CurPhase && m_CurLife < m_MaxLife * (2.f / 3.f))
    {
        m_CurPhase = 2;
        ChangeState(LIFE_STATE::SecondPhase);
    }
    else if (2 == m_CurPhase && m_CurLife < m_MaxLife * (1.f / 3.f))
    {
        m_CurPhase = 3;
        ChangeState(LIFE_STATE::ThirdPhase);
    }
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

            if (_DamageAmount >= 50.f)
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
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_01.wav", 1, 0.5f);

        // UI 설정
        SpawnBossUI(BOSS_TYPE::LIFE);
    }
    break;
    case LIFE_STATE::SecondPhase: {
        StopWalking();
        Animator2D()->Play(L"W09_Boss_NatalieT_Intro", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_02.wav", 1, 0.5f);
        GamePlayStatic::Play2DBGM(L"sound\\BackGround\\W9\\HAND_Music_Natalite_P2_165PBM_070223.wav", 0.2f);
    }
    break;
    case LIFE_STATE::ThirdPhase: {
        StopWalking();
        m_CurLife = m_MaxLife;
        DamageLifeBarUpdate();
        Animator2D()->Play(L"W09_Boss_NatalieT_Intro", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_03.wav", 1, 0.5f);
        GamePlayStatic::Play2DBGM(L"sound\\BackGround\\W9\\HAND_Music_Natalite_P3_165PBM_070223.wav", 0.2f);
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
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Atk_Lgt_01.wav", 1, 0.5f);
    }
    break;
    case LIFE_STATE::Stun: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Stun", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_HitBy_Hvy_01.wav", 1, 0.5f);
    }
    break;
    case LIFE_STATE::Uturn: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Uturn", false);
    }
    break;
    case LIFE_STATE::Attack1: {
        SpawnExclamationMark();
        StopMoving();
        m_bAttackStart = false;
        m_bAttackEnd = false;
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack01", false);
    }
    break;
    case LIFE_STATE::Attack2: {
        SpawnExclamationMark();
        StopMoving();
        m_bAttackStart = false;
        m_bAttackEnd = false;
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack02", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_Hammer_Charge_00.wav", 1, 0.5f);
    }
    break;
    case LIFE_STATE::Attack3: {
        SpawnExclamationMark();
        StopMoving();
        m_bAttackStart = false;
        m_bAttackEnd = false;
        Rigidbody2D()->SetGravityScale(0.f);
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack03", false);
    }
    break;
    case LIFE_STATE::Attack4: {
        SpawnExclamationMark();
        StopMoving();
        m_bAttackStart = false;
        m_bAttackEnd = false;

        Vec2 Vel = Vec2();
        if (DIRECTION_TYPE::LEFT == m_Dir)
        {
            Vel = Vec2(0.3f, 1.f);
        }
        else if (DIRECTION_TYPE::RIGHT == m_Dir)
        {
            Vel = Vec2(-0.3f, 1.f);
        }

        Vel.Normalize();

        Rigidbody2D()->AddForce(Vel * 225.f, ForceMode2D::Impulse);

        Animator2D()->Play(L"W09_Boss_NatalieT_Attack05", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Wings_flap_02.wav", 1, 0.5f);
    }
    break;
    case LIFE_STATE::Attack5: {
        SpawnExclamationMark();
        StopMoving();
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack08", false);
    }
    break;
    case LIFE_STATE::Skill1: {
        StopMoving();
        Rigidbody2D()->SetGravityScale(0.f);
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack06", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_Lifesteal.wav", 1, 0.5f);
    }
    break;
    case LIFE_STATE::Skill2: {
        StopMoving();
        Rigidbody2D()->SetGravityScale(0.f);
        Animator2D()->Play(L"W09_Boss_NatalieT_Attack07", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_LooseEverything.wav", 1, 0.5f);
    }
    break;
    case LIFE_STATE::Death: {
        Animator2D()->Play(L"W09_Boss_NatalieT_Death", false);
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_02.wav", 1, 0.5f);

        Ptr<CSound> pBG = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\BackGround\\W9\\HAND_Music_Natalite_P3_165PBM_070223.wav");
        if (nullptr != pBG)
            pBG->Stop();

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
        StopMoving();
        Rigidbody2D()->SetGravityScale(1.f);
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
        Rigidbody2D()->SetGravityScale(1.f);
    }
    break;
    case LIFE_STATE::Skill2: {
        Rigidbody2D()->SetGravityScale(1.f);
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

    static bool bSoundPlay = false;
    if (!bSoundPlay && !CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_01.wav")->IsPlaying())
    {
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_MakeUp_01.wav", 1, 0.5f);
        bSoundPlay = true;
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(LIFE_STATE::Idle);
        bSoundPlay = true;
    }
}

void CLifeScript::SecondPhase()
{
    static bool bSoundPlay = false;
    if (!bSoundPlay && !CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_02.wav")->IsPlaying())
    {
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_MakeUp_02.wav", 1, 0.5f);
        bSoundPlay = true;
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(LIFE_STATE::Skill1);
        bSoundPlay = true;
    }
}

void CLifeScript::ThirdPhase()
{
    static bool bSoundPlay = false;
    if (!bSoundPlay && !CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Rage_03.wav")->IsPlaying())
    {
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_MakeUp_03.wav", 1, 0.5f);
        bSoundPlay = true;
    }

    if (Animator2D()->IsFinish())
    {
        bSoundPlay = true;
        ChangeState(LIFE_STATE::Skill2);
    }
}

void CLifeScript::Idle()
{
    StopWalking();

    m_PassedTime += DT;

    if (m_PassedTime > 0.3f)
    {
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
            int AttackState = GetRandomInt(1, 4);

            Vec3 origin = Transform()->GetWorldPos();
            RaycastHit2D Hit =
                CPhysics2DMgr::GetInst()->RayCast(Vec2(origin.x, origin.y), Vec2(0.f, -1.f), 125.f, L"Ground"); // Ground 레이어와 충돌체크
            if (nullptr == Hit.pCollisionObj)
            {
                AttackState = GetRandomInt(3, 3); // 공중에 있는 상태라면 특정 공격 상태만 설정
            }

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
        }
        else
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
        int AttackState = GetRandomInt(1, 4);

        Vec3 origin = Transform()->GetWorldPos();
        RaycastHit2D Hit = CPhysics2DMgr::GetInst()->RayCast(Vec2(origin.x, origin.y), Vec2(0.f, -1.f), 125.f, L"Ground"); // Ground 레이어와 충돌체크
        if (nullptr == Hit.pCollisionObj)
        {
            AttackState = GetRandomInt(3, 3); // 공중에 있는 상태라면 특정 공격 상태만 설정
        }

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
    if (!m_bAttackStart && 11 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true, L"Attack1_HitBox");
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_Sword_01.wav", 1, 0.5f);
        m_bAttackStart = true;

        Vec2 Dir = Vec2();

        if (DIRECTION_TYPE::RIGHT == m_Dir)
            Dir = Vec2(1.f, 0.f);
        else
            Dir = Vec2(-1.f, 0.f);

        Rigidbody2D()->AddForce(Dir * GetRandomfloat(400.f, 800.f), ForceMode2D::Impulse);
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

    if (Animator2D()->IsFinish())
    {
        ChangeState(LIFE_STATE::Idle);
    }
}

void CLifeScript::Attack2()
{
    static bool bShaked = false;

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
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_Hammer_Impact_01.wav", 1, 0.5f);
        bShaked = true;
    }
    else if (!m_bAttackEnd && 51 == CurAnimFrm || !m_bAttackEnd && 93 == CurAnimFrm)
    {
        StopMoving();
        Rigidbody2D()->SetGravityScale(1.f);
        SetHitBox(false, L"Attack2_HitBox");
        m_bAttackEnd = true;
    }

    if (Animator2D()->IsFinish())
    {
        ChangeState(LIFE_STATE::Idle);
        bShaked = false;
    }
}
void CLifeScript::Attack3()
{
    if (!m_bAttackStart && 11 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true, L"Attack3_HitBox");

        Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
        Vec3 pos = Transform()->GetWorldPos();
        TargetPos.z = 0.f;
        pos.z = 0.f;
        Vec3 Dist = TargetPos - pos;
        Dist.Normalize();

        Rigidbody2D()->AddForce(Vec2(Dist.x, Dist.y) * GetRandomfloat(200.f, 250.f), ForceMode2D::Impulse);

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

        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_Sword_Spin_LP.wav", 1, 0.5f);
        m_bAttackStart = true;
    }

    if (!m_bAttackEnd && 27 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        StopMoving();
        Rigidbody2D()->SetGravityScale(1.f);
        m_bAttackEnd = true;
    }

    if (Animator2D()->IsFinish())
        ChangeState(LIFE_STATE::Idle);
}

void CLifeScript::Attack4()
{
    if (!m_bAttackStart && 24 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        // 깃털 발사
        if (nullptr != m_pFeatherProjPref)
        {
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            int EffectIdx = pCurLevel->FindLayerIndexByName(L"HitBox");
            if (-1 == EffectIdx)
                EffectIdx = 0;

            for (int i = 0; i < 6; i++)
            {
                CGameObject* pProjectile = m_pFeatherProjPref->Instantiate();
                pProjectile->Transform()->SetRelativePos(Transform()->GetRelativePos());

                // 초기 방향 설정
                Vec3 Vel = Vec3(-1.f, 0.f, 0.f);
                float dTheta = XM_PI / 6.f;
                Vel = Vec3::Transform(Vel, Matrix::CreateRotationZ(-dTheta * float(i)));
                Vel.Normalize();

                CProjectile2DScript* ProjScript = pProjectile->GetScript<CProjectile2DScript>();
                ProjScript->SetTarget(m_pTarget, Vec3(1.f, 0.f, 0.f));
                ProjScript->SetInitDirection(Vel);

                GamePlayStatic::SpawnGameObject(pProjectile, EffectIdx);

                wstring SoundName = L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Atk_Feather_flying_0";
                SoundName += std::to_wstring(i + 1);
                SoundName += L".wav";
                GamePlayStatic::Play2DSound(SoundName.c_str(), 1, 0.5f);
            }
        }

        m_bAttackStart = true;
    }

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
    if (Animator2D()->GetCurAnim()->GetCurFrmIdx() < 23)
        Rigidbody2D()->SetVelocity(Vec2(0.f, 3.f));
    else
        StopMoving();

    static bool bSoundPlay = false;
    if (!bSoundPlay && Animator2D()->GetCurAnim()->GetCurFrmIdx() == 20)
    {
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Vo_Incantation_01.wav", 1, 0.5f);
        bSoundPlay = true;
    }

    if (Animator2D()->IsFinish())
    {
        Vec3 TargetPos = m_pTarget->Transform()->GetWorldPos();
        Vec3 pos = Transform()->GetWorldPos();
        TargetPos.z = 0.f;
        pos.z = 0.f;
        Vec3 Dist = TargetPos - pos;

        // 공격 범위 이내에 존재한다면 1피
        if (Dist.Length() < m_AttackRange)
        {
            CPlayerScript* pPlayerScript = m_pTarget->GetScript<CPlayerScript>();

            if (nullptr != pPlayerScript)
            {
                pPlayerScript->TakeHit(pPlayerScript->GetCurLife() - 1);
            }
        }

        ChangeState(LIFE_STATE::Idle);
    }
}

void CLifeScript::Skill2()
{
    if (Animator2D()->GetCurAnim()->GetCurFrmIdx() < 40)
        Rigidbody2D()->SetVelocity(Vec2(0.f, 3.f));
    else
        StopMoving();

    if (Animator2D()->IsFinish())
    {
        CPlayerScript* pPlayerScript = m_pTarget->GetScript<CPlayerScript>();

        if (nullptr != pPlayerScript)
        {
            // Scythe 이외 모든 무기 삭제
            pPlayerScript->ChangeWeapon(PLAYER_WEAPON_CLOAK::NONE);
            pPlayerScript->ChangeWeapon(PLAYER_WEAPON_SPELL::NONE);
        }

        ChangeState(LIFE_STATE::Idle);
    }
}

void CLifeScript::Death()
{
    StopWalking();

    static bool bSoundPlay = false;
    if (!bSoundPlay && Animator2D()->GetCurAnim()->GetCurFrmIdx() == 46)
    {
        GamePlayStatic::Play2DSound(L"sound\\Boss\\boss_w09_nathality\\NPC_NathalieT_Death_SfxVo_01.wav", 1, 0.5f);
        bSoundPlay = true;
    }

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
