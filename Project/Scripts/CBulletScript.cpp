#include "pch.h"
#include "CBulletScript.h"

CBulletScript::CBulletScript()
    : CScript(BULLETSCRIPT)
    , m_InitVelocity()
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::begin()
{
    if (Rigidbody())
    {
        Rigidbody()->AddForce(m_InitVelocity, ForceMode::Impulse); // Impule로 한번만 강한 힘을 적용
    }
}
