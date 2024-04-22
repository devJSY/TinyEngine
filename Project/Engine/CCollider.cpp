#include "pch.h"
#include "CCollider.h"
#include "CScript.h"
#include "CTransform.h"
#include "physx\\PxPhysicsAPI.h"

CCollider::CCollider(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_RuntimeShape(nullptr)
    , m_bTrigger(false)
    , m_Mtrl(nullptr)
    , m_Center(Vec3())
    , m_CollisionCount(0)
    , m_TriggerCount(0)
{
}

CCollider::CCollider(const CCollider& origin)
    : CComponent(origin)
    , m_RuntimeShape(nullptr)
    , m_bTrigger(origin.m_bTrigger)
    , m_Mtrl(origin.m_Mtrl)
    , m_Center(origin.m_Center)
    , m_CollisionCount(0)
    , m_TriggerCount(0)
{
}

CCollider::~CCollider()
{
}

void CCollider::finaltick()
{
    if (nullptr == m_RuntimeShape)
        return;

    // 트랜스폼 위치 정보 업데이트
    physx::PxShape* shape = (physx::PxShape*)m_RuntimeShape;
    physx::PxRigidActor* body = shape->getActor();
    physx::PxTransform PxTr = body->getGlobalPose();

    Vec3 WolrdPos = Transform()->GetWorldPos();
    PxTr.p = physx::PxVec3(WolrdPos.x, WolrdPos.y, WolrdPos.z);
    body->setGlobalPose(PxTr);
}

void CCollider::OnCollisionEnter(CCollider* _OtherCollider)
{
    ++m_CollisionCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnCollisionEnter(_OtherCollider);
}

void CCollider::OnCollisionStay(CCollider* _OtherCollider)
{
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnCollisionStay(_OtherCollider);
}

void CCollider::OnCollisionExit(CCollider* _OtherCollider)
{
    --m_CollisionCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnCollisionExit(_OtherCollider);
}

void CCollider::OnTriggerEnter(CCollider* _OtherCollider)
{
    ++m_TriggerCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerEnter(_OtherCollider);
}

void CCollider::OnTriggerStay(CCollider* _OtherCollider)
{
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerStay(_OtherCollider);
}

void CCollider::OnTriggerExit(CCollider* _OtherCollider)
{
    --m_TriggerCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerExit(_OtherCollider);
}

void CCollider::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_bTrigger, sizeof(bool), 1, _File);
    SaveAssetRef(m_Mtrl, _File);
    fwrite(&m_Center, sizeof(Vec3), 1, _File);
}

void CCollider::LoadFromLevelFile(FILE* _File)
{
    fread(&m_bTrigger, sizeof(bool), 1, _File);
    LoadAssetRef(m_Mtrl, _File);
    fread(&m_Center, sizeof(Vec3), 1, _File);
}
