#include "pch.h"
#include "CCollider.h"
#include "CScript.h"
#include "physx\\PxPhysicsAPI.h"

#include "CPhysicsMgr.h"

#include "CTransform.h"

CCollider::CCollider(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_RuntimeShape(nullptr)
    , m_bTrigger(false)
    , m_Mtrl(nullptr)
    , m_Center(Vec3())
    , m_CollisionCount(0)
    , m_TriggerCount(0)
    , m_bEnabled(true)
    , m_PrevScale(Vec3())
    , m_NextCenter(Vec3())
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
    , m_bEnabled(origin.m_bEnabled)
    , m_PrevScale(Vec3())
    , m_NextCenter(origin.m_NextCenter)
{
}

CCollider::~CCollider()
{
}

void CCollider::begin()
{
    m_PrevScale = Transform()->GetTransformWorldScale();
}

void CCollider::finaltick()
{
    m_Center = m_NextCenter;

    // 트랜스폼의 스케일이 변경되었다면 재생성
    Vec3 TransformWorldScale = Transform()->GetTransformWorldScale();
    if ((m_PrevScale - TransformWorldScale).Length() > 1e-3f)
    {
        GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
    }

    m_PrevScale = TransformWorldScale;

    if (nullptr == m_RuntimeShape)
        return;

    // 트랜스폼 위치 정보 업데이트
    const float PPM = CPhysicsMgr::GetInst()->GetPPM();

    Vec3 WorldPos = Transform()->GetWorldPos();
    Quat WorldQuat = Transform()->GetWorldQuaternion();

    WorldPos /= PPM;

    physx::PxTransform PxTr = physx::PxTransform(WorldPos, physx::PxQuat(WorldQuat.x, WorldQuat.y, WorldQuat.z, WorldQuat.w));

    physx::PxShape* shape = (physx::PxShape*)m_RuntimeShape;
    physx::PxRigidActor* body = shape->getActor();
    body->setGlobalPose(PxTr);

    physx::PxTransform LocalPos = shape->getLocalPose();
    LocalPos.p = Transform()->GetWorldRatio() * m_Center;
    LocalPos.p /= PPM;
    shape->setLocalPose(LocalPos);
}

void CCollider::SetTrigger(bool _Trigger)
{
    if (m_bTrigger == _Trigger)
        return;

    m_bTrigger = _Trigger;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCollider::SetMaterial(Ptr<CPhysicMaterial> _Mtrl)
{
    if (m_Mtrl == _Mtrl)
        return;

    m_Mtrl = _Mtrl;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCollider::SetCenter(Vec3 _Center)
{
    if (m_NextCenter == _Center)
        return;

    m_NextCenter = _Center;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCollider::SetEnabled(bool _bEnabled)
{
    if (m_bEnabled == _bEnabled)
        return;

    m_bEnabled = _bEnabled;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
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

void CCollider::OnControllerColliderHit(ControllerColliderHit Hit)
{
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnControllerColliderHit(Hit);
}

UINT CCollider::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bTrigger, sizeof(bool), 1, _File);
    MemoryByte += SaveAssetRef(m_Mtrl, _File);
    fwrite(&m_Center, sizeof(Vec3), 1, _File);
    fwrite(&m_bEnabled, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CCollider::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bTrigger, sizeof(bool), 1, _File);
    MemoryByte += LoadAssetRef(m_Mtrl, _File);
    fread(&m_Center, sizeof(Vec3), 1, _File);
    fread(&m_bEnabled, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(bool);

    m_NextCenter = m_Center;

    return MemoryByte;
}
