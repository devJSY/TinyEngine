#include "pch.h"
#include "CKirbyBulletScript.h"

CKirbyBulletScript::CKirbyBulletScript()
    : CScript(KIRBYBULLETSCRIPT)
    , m_InitVelocity()
{
}

CKirbyBulletScript::~CKirbyBulletScript()
{
}

void CKirbyBulletScript::begin()
{
    if (Rigidbody())
    {
        Rigidbody()->AddForce(m_InitVelocity, ForceMode::Impulse);
    }
}
