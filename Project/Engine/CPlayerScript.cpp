#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
    : m_Speed(1000.f)
{
}

CPlayerScript::~CPlayerScript()
{
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
