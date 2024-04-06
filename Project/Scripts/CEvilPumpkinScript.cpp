#include "pch.h"
#include "CEvilPumpkinScript.h"
#include "CPlayerCameraScript.h"
#include "CCinematicScript.h"

CEvilPumpkinScript::CEvilPumpkinScript()
    : CEnemyScript(EVILPUMPKINSCRIPT)
    , m_State(EVILPUMPKINSCRIPT_STATE::Hide)
    , m_AttackCount(0)
    , m_PassedTime(0.f)
{
    m_Life = 1100;
    m_Speed = 3;
    m_ATK = 15;
    m_AttackRange = 200.f;
}

CEvilPumpkinScript::CEvilPumpkinScript(const CEvilPumpkinScript& origin)
    : CEnemyScript(origin)
    , m_State(origin.m_State)
    , m_AttackCount(origin.m_AttackCount)
    , m_PassedTime(0.f)
{
}

CEvilPumpkinScript::~CEvilPumpkinScript()
{
}

void CEvilPumpkinScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DGlowMtrl"));

        Transform()->SetRelativeScale(Vec3(2048.f, 2048.f, 1.f));
    }

    Ptr<CMaterial> pMtrl = MeshRender()->CreateDynamicMaterial();

    pMtrl->SetScalarParam(INT_0, 0);
    pMtrl->SetScalarParam(FLOAT_0, 0.f);
    pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 0.f, 0.f, 1.f));

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Attack01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Attack02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Attack03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Attack04.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Attack05.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Death.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Hit.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Intro.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Run.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Stun.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
    m_Dir = DIRECTION_TYPE::LEFT;
    RotateTransform();

    // 타겟 등록
    m_pTarget = CGameManagerScript::GetInset()->GetPlayer();
}

void CEvilPumpkinScript::tick()
{
    CEnemyScript::tick();

    // FSM
    switch (m_State)
    {
    case EVILPUMPKINSCRIPT_STATE::Hide:
        Hide();
        break;
    case EVILPUMPKINSCRIPT_STATE::Intro:
        Intro();
        break;
    case EVILPUMPKINSCRIPT_STATE::Idle:
        Idle();
        break;
    case EVILPUMPKINSCRIPT_STATE::Run:
        Run();
        break;
    case EVILPUMPKINSCRIPT_STATE::Hit:
        Hit();
        break;
    case EVILPUMPKINSCRIPT_STATE::Stun:
        Stun();
        break;
    case EVILPUMPKINSCRIPT_STATE::Attack:
        Attack();
        break;
    case EVILPUMPKINSCRIPT_STATE::Death:
        Death();
        break;
    }
}

void CEvilPumpkinScript::TakeHit(int _DamageAmount, Vec3 _Hitdir)
{
    if (EVILPUMPKINSCRIPT_STATE::Death == m_State || EVILPUMPKINSCRIPT_STATE::Hide == m_State || EVILPUMPKINSCRIPT_STATE::Intro == m_State)
        return;

    m_Life -= _DamageAmount;

    if (m_Life <= 0)
        ChangeState(EVILPUMPKINSCRIPT_STATE::Death);
    else
    {
        if (EVILPUMPKINSCRIPT_STATE::Attack != m_State && EVILPUMPKINSCRIPT_STATE::Stun != m_State && EVILPUMPKINSCRIPT_STATE::Hit != m_State)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (_DamageAmount >= 15.f)
                ChangeState(EVILPUMPKINSCRIPT_STATE::Stun);
            else if (_DamageAmount >= 12.f)
                ChangeState(EVILPUMPKINSCRIPT_STATE::Hit);
        }
    }
}

void CEvilPumpkinScript::ChangeState(EVILPUMPKINSCRIPT_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CEvilPumpkinScript::EnterState()
{
    switch (m_State)
    {
    case EVILPUMPKINSCRIPT_STATE::Hide: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Intro: {
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Intro", false);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Idle: {
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Idle", true);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Run: {
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Run", true);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Hit: {
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Hit", false);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Stun: {
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Stun", false);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Attack: {
        // 4는 사용 X
        if (4 == m_AttackCount)
            m_AttackCount++;

        if (1 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack01", false);
        else if (2 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack02", false);
        else if (3 == m_AttackCount)
        {
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack03", false);
            SetHitBox(true, L"Attack3_1_HitBox");

            const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
            for (size_t i = 0; i < vecChild.size(); i++)
            {
                if (L"Attack03_FX" == vecChild[i]->GetName())
                {
                    vecChild[i]->Animator2D()->Play(L"Miniboss_EvilPumpkin_Fist", false);
                }
            }
        }
        // else if (4 == m_AttackCount)
        //     Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack04", false);
        else if (5 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack05", false);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Death: {
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Death", false);
    }
    break;
    }
}

void CEvilPumpkinScript::ExitState()
{
    switch (m_State)
    {
    case EVILPUMPKINSCRIPT_STATE::Hide: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Intro: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Idle: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Run: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Hit: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Stun: {
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Attack: {
        SetHitBox(false);
    }
    break;
    case EVILPUMPKINSCRIPT_STATE::Death: {
    }
    break;
    }
}

void CEvilPumpkinScript::Hide()
{
}

void CEvilPumpkinScript::Intro()
{
    StopWalking();

    // 206 부터 Glow On
    if (Animator2D()->GetCurAnim()->GetCurFrmIdx() >= 206)
    {
        MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1);
        MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.5f);
        MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 0.2f, 0.f, 1.f));
    }

    if (Animator2D()->IsFinish())
        ChangeState(EVILPUMPKINSCRIPT_STATE::Idle);
}

void CEvilPumpkinScript::Idle()
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
        m_AttackCount = GetRandomInt(1, 5);
        ChangeState(EVILPUMPKINSCRIPT_STATE::Attack);

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

    if (m_PassedTime > 1.f)
    {
        ChangeState(EVILPUMPKINSCRIPT_STATE::Run);

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

void CEvilPumpkinScript::Run()
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
        m_AttackCount = GetRandomInt(1, 5);
        ChangeState(EVILPUMPKINSCRIPT_STATE::Attack);

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

    if (m_PassedTime > 1.f)
    {
        ChangeState(EVILPUMPKINSCRIPT_STATE::Idle);

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

void CEvilPumpkinScript::Hit()
{
    if (Animator2D()->IsFinish())
        ChangeState(EVILPUMPKINSCRIPT_STATE::Idle);
}

void CEvilPumpkinScript::Stun()
{
    if (Animator2D()->IsFinish())
        ChangeState(EVILPUMPKINSCRIPT_STATE::Idle);
}

void CEvilPumpkinScript::Attack()
{
    static bool HasAttack = false;

    StopWalking();

    if (Animator2D()->IsFinish())
    {
        ChangeState(EVILPUMPKINSCRIPT_STATE::Idle);
        HasAttack = false;
    }

    if (!HasAttack && 2 == m_AttackCount && 29 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true, L"Attack2_HitBox");
        HasAttack = true;
    }
    else if (!HasAttack && 3 == m_AttackCount && 40 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(false, L"Attack3_1_HitBox");
        SetHitBox(true, L"Attack3_2_HitBox");
        HasAttack = true;
    }
    else if (!HasAttack && 5 == m_AttackCount && 30 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        SetHitBox(true, L"Attack5_HitBox");

        const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
        for (size_t i = 0; i < vecChild.size(); i++)
        {
            if (L"Attack05_FX" == vecChild[i]->GetName())
            {
                vecChild[i]->Animator2D()->Play(L"Miniboss_EvilPumpkin_FX_Attack05", false);
            }
        }
    }
}

void CEvilPumpkinScript::Death()
{
    StopWalking();

    // 206 부터 Glow Off
    if (Animator2D()->GetCurAnim()->GetCurFrmIdx() >= 40)
    {
        MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
    }

    if (Animator2D()->IsFinish())
    {
        // Player Camera Lock Off
        CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
        if (nullptr != pPlayerCamObj)
        {
            CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
            if (nullptr != pScript)
            {
                pScript->Lock(false);
            }
        }

        // Wall Collider Disable
        CGameObject* pCinematicObject = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"CinematicObject");
        if (nullptr != pCinematicObject)
        {
            CCinematicScript* pCinematicScript = pCinematicObject->GetScript<CCinematicScript>();
            if (nullptr != pCinematicScript)
            {
                pCinematicScript->SetEnableWallCollider(false);
            }
        }

        ChangeState(EVILPUMPKINSCRIPT_STATE::Hide);
    }
}

void CEvilPumpkinScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
}

void CEvilPumpkinScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
}
