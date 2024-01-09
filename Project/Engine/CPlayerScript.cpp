#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
    : CScript(SCRIPT_TYPE::PLAYERSCRIPT)
    , m_Speed(1000.f)
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

    Animator2D()->Play(L"dodge_back", true);
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
