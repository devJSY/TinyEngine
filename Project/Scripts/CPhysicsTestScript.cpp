#include "pch.h"
#include "CPhysicsTestScript.h"

CPhysicsTestScript::CPhysicsTestScript()
    : CScript(PHYSICSTESTSCRIPT)
    , m_Force(Vec3())
    , m_Torque(Vec3())
    , m_ForceMode(0)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_Force, "Force");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_Torque, "Torque");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ForceMode, "Force Mode");
}

CPhysicsTestScript::~CPhysicsTestScript()
{
}

void CPhysicsTestScript::tick()
{
    if (nullptr == Rigidbody())
        return;

    if (KEY_TAP(KEY::SPACE))
    {
        if (0 == m_ForceMode)
        {
            Rigidbody()->AddForce(m_Force);
        }
        else if (1 == m_ForceMode)
        {
            Rigidbody()->AddForce(m_Force, ForceMode::Acceleration);
        }
        else if (2 == m_ForceMode)
        {
            Rigidbody()->AddForce(m_Force, ForceMode::Impulse);
        }
        else if (3 == m_ForceMode)
        {
            Rigidbody()->AddForce(m_Force, ForceMode::VelocityChange);
        }
    }

    if (KEY_TAP(KEY::S))
    {
        if (0 == m_ForceMode)
        {
            Rigidbody()->AddTorque(m_Torque);
        }
        else if (1 == m_ForceMode)
        {
            Rigidbody()->AddTorque(m_Torque, ForceMode::Acceleration);
        }
        else if (2 == m_ForceMode)
        {
            Rigidbody()->AddTorque(m_Torque, ForceMode::Impulse);
        }
        else if (3 == m_ForceMode)
        {
            Rigidbody()->AddTorque(m_Torque, ForceMode::VelocityChange);
        }
    }
}
