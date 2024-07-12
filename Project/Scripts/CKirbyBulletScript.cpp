#include "pch.h"
#include "CKirbyBulletScript.h"

CKirbyBulletScript::CKirbyBulletScript()
    : CScript(KIRBYBULLETSCRIPT)
    , m_PlayTime(5.f)
{
}

CKirbyBulletScript::~CKirbyBulletScript()
{
}

void CKirbyBulletScript::begin()
{
    if (!Rigidbody())
    {
        GetOwner()->AddComponent(new CRigidbody());
    }

    Rigidbody()->AddForce(m_InitVelocity, ForceMode::Impulse);
}

void CKirbyBulletScript::tick()
{
    m_PlayTime -= DT;

    if (m_PlayTime <= 0.f)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}