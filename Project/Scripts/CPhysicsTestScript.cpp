#include "pch.h"
#include "CPhysicsTestScript.h"
#include <Engine\\CAssetMgr.h>
#include <Engine\\CPhysicsMgr.h>

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

void CPhysicsTestScript::begin()
{
}

void CPhysicsTestScript::tick()
{
    // RayCast Test
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 10.f, L"RayCastTest");
    Vec3 Color = Hit.pCollisionObj == nullptr ? Vec3(0.f, 1.f, 0.f) : Vec3(1.f, 0.f, 0.f);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 10.f, Color, false);

    if (KEY_TAP(KEY::L))
    {
        if (BoxCollider())
            BoxCollider()->SetEnabled(!BoxCollider()->IsEnabled());
        if (SphereCollider())
            SphereCollider()->SetEnabled(!SphereCollider()->IsEnabled());
        if (CapsuleCollider())
            CapsuleCollider()->SetEnabled(!CapsuleCollider()->IsEnabled());

        if (BoxCollider2D())
            BoxCollider2D()->SetEnabled(!BoxCollider2D()->IsEnabled());
        if (CircleCollider2D())
            CircleCollider2D()->SetEnabled(!CircleCollider2D()->IsEnabled());
        if (PolygonCollider2D())
            PolygonCollider2D()->SetEnabled(!PolygonCollider2D()->IsEnabled());
        if (EdgeCollider2D())
            EdgeCollider2D()->SetEnabled(!EdgeCollider2D()->IsEnabled());
    }

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
