#include "pch.h"
#include "CScriptMgr.h"
#include "CPlayerScript.h"
#include "CBulletScript.h"

CPlayerScript::CPlayerScript()
    : CScript(SCRIPT_TYPE::PLAYERSCRIPT)
    , m_Speed(100.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_back.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_backward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_forward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_front.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_back.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_back_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_backward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_forward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_forward_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_front.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_front_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_back.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_back_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_backward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_backward_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_doorway.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_forward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_forward_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_front.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_front_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_choose.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_idle.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_smoking_idle.anim");

    Animator2D()->Play(L"select_idle", true);
}

void CPlayerScript::tick()
{
    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vRot = Transform()->GetRelativeRotation();

    if (KEY_PRESSED(KEY::UP))
    {
        vPos.y += DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::DOWN))
    {
        vPos.y -= DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::LEFT))
    {
        vPos.x -= DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::RIGHT))
    {
        vPos.x += DT * m_Speed;
    }

    /* if (KEY_PRESSED(KEY::NUM1))
     {
         vRot.x += DT * XM_PI;
     }

     if (KEY_PRESSED(KEY::NUM2))
     {
         vRot.y += DT * XM_PI;
     }

     if (KEY_PRESSED(KEY::NUM3))
     {
         vRot.z += DT * XM_PI;
     }*/

    // °øÆ÷Åº »ý¼º
    if (KEY_TAP(KEY::Q))
    {
        /*CGameObject* pBullet = new CGameObject;
        pBullet->SetName(L"Bullet");

        pBullet->AddComponent(new CTransform);
        pBullet->AddComponent(new CCollider2D);
        pBullet->AddComponent(new CMeshRender);
        pBullet->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::BULLETSCRIPT));

        pBullet->Transform()->SetRelativePos(Transform()->GetRelativePos());
        pBullet->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

        pBullet->Collider2D()->SetColliderType(COLLIDER2D_TYPE::CIRCLE);
        pBullet->Collider2D()->SetRadius(100.f);
        pBullet->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        pBullet->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BasicMtrl"));

        pBullet->GetScript<CBulletScript>()->SetDir(Transform()->GetWorldDir(DIR_TYPE::RIGHT));

        GamePlayStatic::SpawnGameObject(pBullet, 4);*/

        // °øÆ÷Åº
        CGameObject* pShockWave = new CGameObject;
        pShockWave->SetName(L"Shock Wave");
        pShockWave->AddComponent(new CTransform);
        pShockWave->AddComponent(new CMeshRender);
        pShockWave->AddComponent(new CCollider2D);
        pShockWave->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::SHOCKWAVESCRIPT));

        Vec3 pos = Transform()->GetRelativePos();
        pShockWave->Transform()->SetRelativePos(pos);
        pShockWave->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1.f));
        pShockWave->Collider2D()->SetColliderType(COLLIDER2D_TYPE::CIRCLE);
        pShockWave->Collider2D()->SetRadius(1.f);
        pShockWave->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        pShockWave->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShockWaveMtrl"));

        GamePlayStatic::SpawnGameObject(pShockWave, 14);
    }

    Transform()->SetRelativePos(vPos);
    Transform()->SetRelativeRotation(vRot);
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}
