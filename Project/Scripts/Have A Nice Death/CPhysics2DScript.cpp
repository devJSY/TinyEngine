#include "pch.h"
#include "CPhysics2DScript.h"

CPhysics2DScript::CPhysics2DScript()
    : CScript(PHYSICS2DSCRIPT)
{
}

CPhysics2DScript::~CPhysics2DScript()
{
}

void CPhysics2DScript::tick()
{
    if (nullptr == Rigidbody2D())
        return;

    if (KEY_TAP(KEY::SPACE))
    {
        Rigidbody2D()->AddForce(Vec2(0.f, 10.f), ForceMode2D::Impulse);
    }

    if (KEY_TAP(KEY::S))
    {
        Rigidbody2D()->AddTorque(10.f, ForceMode2D::Impulse);
    }
}
