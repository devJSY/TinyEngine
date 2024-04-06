#include "pch.h"
#include "CEvilPumpkinScript.h"
#include "CPlayerCameraScript.h"
#include "CCinematicScript.h"

CEvilPumpkinScript::CEvilPumpkinScript()
    : CEnemyScript(EVILPUMPKINSCRIPT)
    , m_State(EVILPUMPKINSCRIPT_STATE::Hide)
    , m_AttackCount(0)
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
        if (m_State != EVILPUMPKINSCRIPT_STATE::Attack)
        {
            StopWalking();
            Vec2 Force = Vec2(_Hitdir.x, _Hitdir.y);
            Force.Normalize();
            Force *= Rigidbody2D()->GetMass();
            Rigidbody2D()->AddForce(Force, ForceMode2D::Impulse);

            if (_DamageAmount > 20.f)
                ChangeState(EVILPUMPKINSCRIPT_STATE::Stun);
            else
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
        if (1 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack01", false);
        else if (2 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack02", false);
        else if (3 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack03", false);
        else if (4 == m_AttackCount)
            Animator2D()->Play(L"Miniboss_EvilPumpkin_Attack04", false);
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

    // 206 บฮลอ Glow On
    if (Animator2D()->GetCurAnim()->GetCurFrmIdx() >= 206)
    {
        MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
        MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.f);
        MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 0.f, 0.f, 1.f));
    }

    if (Animator2D()->IsFinish())
        ChangeState(EVILPUMPKINSCRIPT_STATE::Idle);
}

void CEvilPumpkinScript::Idle()
{
}

void CEvilPumpkinScript::Run()
{
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
}

void CEvilPumpkinScript::Death()
{
    StopWalking();

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

void CEvilPumpkinScript::OnDetectTargetEnter(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetEnter(_TargetObj);
}

void CEvilPumpkinScript::OnDetectTargetExit(CGameObject* _TargetObj)
{
    CEnemyScript::OnDetectTargetExit(_TargetObj);
}

void CEvilPumpkinScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
}

void CEvilPumpkinScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
}
