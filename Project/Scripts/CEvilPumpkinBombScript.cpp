#include "pch.h"
#include "pch.h"
#include "CEvilPumpkinBombScript.h"

CEvilPumpkinBombScript::CEvilPumpkinBombScript()
    : CEnemyScript(EVILPUMPKINBOMBSCRIPT)
    , m_State(EVILPUMPKINBOMB_STATE::Appear)
    , m_bInitForce(false)
{
    m_ATK = 5;
}

CEvilPumpkinBombScript::~CEvilPumpkinBombScript()
{
}

void CEvilPumpkinBombScript::begin()
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
    pMtrl->SetScalarParam(FLOAT_0, 0.4f);
    pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 0.2f, 0.f, 1.f));

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Bomb_Appear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Miniboss\\Miniboss_EvilPumpkin\\Miniboss_EvilPumpkin_Bomb_Explode.anim");
    }

    if (nullptr == Rigidbody2D())
        GetOwner()->AddComponent(new CRigidbody2D);

    ChangeState(m_State);
}

void CEvilPumpkinBombScript::tick()
{
    CEnemyScript::tick();

    switch (m_State)
    {
    case EVILPUMPKINBOMB_STATE::Appear:
        Appear();
        break;
    case EVILPUMPKINBOMB_STATE::Explode:
        Explode();
        break;
    }
}

void CEvilPumpkinBombScript::ChangeState(EVILPUMPKINBOMB_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CEvilPumpkinBombScript::EnterState()
{
    switch (m_State)
    {
    case EVILPUMPKINBOMB_STATE::Appear: {
        SetHitBox(true);
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Bomb_Appear", false);
    }
    break;
    case EVILPUMPKINBOMB_STATE::Explode: {
        SetHitBox(true);
        Animator2D()->Play(L"Miniboss_EvilPumpkin_Bomb_Explode", false);
    }
    break;
    }
}

void CEvilPumpkinBombScript::ExitState()
{
    switch (m_State)
    {
    case EVILPUMPKINBOMB_STATE::Appear: {
    }
    break;
    case EVILPUMPKINBOMB_STATE::Explode: {
    }
    break;
    }
}

void CEvilPumpkinBombScript::Appear()
{
    if (!m_bInitForce && 1 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
    {
        if (m_Dir == DIRECTION_TYPE::RIGHT)
            Rigidbody2D()->AddForce(Vec2(GetRandomfloat(10.f, 20.f), 20.f), ForceMode2D::Impulse);
        else
            Rigidbody2D()->AddForce(Vec2(GetRandomfloat(-10.f, -20.f), 20.f), ForceMode2D::Impulse);

        m_bInitForce = true;
    }

    if (Animator2D()->IsFinish())
        ChangeState(EVILPUMPKINBOMB_STATE::Explode);
}

void CEvilPumpkinBombScript::Explode()
{
    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
        GamePlayStatic::Play2DSound(L"sound\\MiniBoss\\miniboss_w01_pumpquinn\\Npc_PumpEvil_Proj_Pumpkin_Explo_01.wav", 1, 0.5f);
    }
}
