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

void CPhysicsTestScript::OnCollisionEnter(CCollider* _OtherCollider)
{
    LOG(Log, "OnCollisionEnter Other Collider Name : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CPhysicsTestScript::OnCollisionStay(CCollider* _OtherCollider)
{
    LOG(Log, "OnCollisionStay Other Collider Name : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CPhysicsTestScript::OnCollisionExit(CCollider* _OtherCollider)
{
    LOG(Log, "OnCollisionExit Other Collider Name : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CPhysicsTestScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    LOG(Log, "OnTriggerEnter Other Collider Name : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CPhysicsTestScript::OnTriggerStay(CCollider* _OtherCollider)
{
    LOG(Log, "OnTriggerStay Other Collider Name : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CPhysicsTestScript::OnTriggerExit(CCollider* _OtherCollider)
{
    LOG(Log, "OnTriggerExit Other Collider Name : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CPhysicsTestScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Force, sizeof(Vec3), 1, _File);
    fwrite(&m_Torque, sizeof(Vec3), 1, _File);
    fwrite(&m_ForceMode, sizeof(int), 1, _File);
}

void CPhysicsTestScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Force, sizeof(Vec3), 1, _File);
    fread(&m_Torque, sizeof(Vec3), 1, _File);
    fread(&m_ForceMode, sizeof(int), 1, _File);
}
