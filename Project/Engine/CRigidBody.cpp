#include "pch.h"
#include "CRigidBody.h"
#include "physx/PxPhysicsAPI.h"

#include "CPhysicsMgr.h"

#include "CTransform.h"

CRigidbody::CRigidbody()
    : CComponent(COMPONENT_TYPE::RIGIDBODY)
    , m_RuntimeBody(nullptr)
    , m_Mass(1.f)
    , m_Drag(0.f)
    , m_AngularDrag(0.05f)
    , m_bGravity(true)
    , m_bKinematic(false)
    , m_CollisionDetectionType(CollisionDetection_TYPE::Discrete)
    , m_FreezePosition{}
    , m_FreezeRotation{}
{
}

CRigidbody::CRigidbody(const CRigidbody& origin)
    : CComponent(origin)
    , m_RuntimeBody(nullptr)
    , m_Mass(origin.m_Mass)
    , m_Drag(origin.m_Drag)
    , m_AngularDrag(origin.m_AngularDrag)
    , m_bGravity(origin.m_bGravity)
    , m_bKinematic(origin.m_bKinematic)
    , m_CollisionDetectionType(origin.m_CollisionDetectionType)
    , m_FreezePosition{origin.m_FreezePosition[0], origin.m_FreezePosition[1], origin.m_FreezePosition[2]}
    , m_FreezeRotation{origin.m_FreezeRotation[0], origin.m_FreezeRotation[1], origin.m_FreezeRotation[2]}
{
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::finaltick()
{
    if (nullptr == m_RuntimeBody)
        return;

    // Ʈ������ ��ġ ���� ������Ʈ
    float PPM = CPhysicsMgr::GetInst()->GetPPM();

    Vec3 WorldPos = Transform()->GetWorldPos();
    Quat WorldQuat = Transform()->GetWorldQuaternion();

    WorldPos /= PPM;

    physx::PxTransform PxTr = physx::PxTransform(WorldPos, physx::PxQuat(WorldQuat.x, WorldQuat.y, WorldQuat.z, WorldQuat.w));

    physx::PxRigidActor* body = (physx::PxRigidActor*)m_RuntimeBody;
    body->setGlobalPose(PxTr);
}

void CRigidbody::AddForce(Vec3 _Force, ForceMode _Mode)
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;

    switch (_Mode)
    {
    case ForceMode::Force:
        body->addForce(physx::PxVec3(_Force.x, _Force.y, _Force.z), physx::PxForceMode::eFORCE);
        break;
    case ForceMode::Acceleration:
        body->addForce(physx::PxVec3(_Force.x, _Force.y, _Force.z), physx::PxForceMode::eACCELERATION);
        break;
    case ForceMode::Impulse:
        body->addForce(physx::PxVec3(_Force.x, _Force.y, _Force.z), physx::PxForceMode::eIMPULSE);
    break;
    case ForceMode::VelocityChange:
        body->addForce(physx::PxVec3(_Force.x, _Force.y, _Force.z), physx::PxForceMode::eVELOCITY_CHANGE);
        break;
    }
}

void CRigidbody::AddTorque(Vec3 _Torque, ForceMode _Mode)
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;

    switch (_Mode)
    {
    case ForceMode::Force:
        body->addTorque(physx::PxVec3(_Torque.x, _Torque.y, _Torque.z), physx::PxForceMode::eFORCE);
        break;
    case ForceMode::Acceleration:
        body->addTorque(physx::PxVec3(_Torque.x, _Torque.y, _Torque.z), physx::PxForceMode::eACCELERATION);
        break;
    case ForceMode::Impulse:
        body->addTorque(physx::PxVec3(_Torque.x, _Torque.y, _Torque.z), physx::PxForceMode::eIMPULSE);
        break;
    case ForceMode::VelocityChange:
        body->addTorque(physx::PxVec3(_Torque.x, _Torque.y, _Torque.z), physx::PxForceMode::eVELOCITY_CHANGE);
        break;
    }
}

float CRigidbody::GetSpeed()
{
    if (nullptr == m_RuntimeBody)
        return 0.f;

    return GetVelocity().Length();
}

Vec3 CRigidbody::GetVelocity()
{
    if (nullptr == m_RuntimeBody)
        return Vec3();

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    return body->getLinearVelocity();
}

void CRigidbody::SetVelocity(Vec3 _vel)
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setLinearVelocity(_vel);
}

Vec3 CRigidbody::GetAngularVelocity()
{
    if (nullptr == m_RuntimeBody)
        return Vec3();

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    return body->getAngularVelocity();
}

void CRigidbody::SetAngularVelocity(Vec3 _vel)
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setAngularVelocity(_vel);
}

Vec3 CRigidbody::GetInertiaTensor()
{
    if (nullptr == m_RuntimeBody)
        return Vec3();

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    return body->getMassSpaceInertiaTensor();
}

void CRigidbody::SetInertiaTensor(Vec3 _Tensor)
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setMassSpaceInertiaTensor(_Tensor);
}

Vec3 CRigidbody::GetLocalCenterOfMass()
{
    if (nullptr == m_RuntimeBody)
        return Vec3();

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    return body->getCMassLocalPose().p;
}

void CRigidbody::SetLocalCenterOfMass(Vec3 _Center)
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    physx::PxTransform PxTr = body->getCMassLocalPose();
    PxTr.p = _Center;
    body->setCMassLocalPose(PxTr);
}

bool CRigidbody::IsSleeping()
{
    if (nullptr == m_RuntimeBody)
        return false;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    return body->isSleeping();
}

void CRigidbody::Sleep()
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->putToSleep();
}

void CRigidbody::WakeUp()
{
    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->wakeUp();
}

void CRigidbody::SetMass(float _Mass)
{
    m_Mass = _Mass;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setMass(m_Mass);
}

void CRigidbody::SetDrag(float _Drag)
{
    m_Drag = _Drag;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setLinearDamping(m_Drag);
}

void CRigidbody::SetAngularDrag(float _Drag)
{
    m_AngularDrag = _Drag;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setAngularDamping(m_AngularDrag);
}

void CRigidbody::SetUseGravity(bool _bGravity)
{
    m_bGravity = _bGravity;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !m_bGravity);
}

void CRigidbody::SetKinematic(bool _kinematic)
{
    m_bKinematic = _kinematic;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;
    body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_bKinematic);
}

void CRigidbody::SetCollisionDetectionType(CollisionDetection_TYPE _ColDet)
{
    m_CollisionDetectionType = _ColDet;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;

    switch (m_CollisionDetectionType)
    {
    case CollisionDetection_TYPE::Discrete: {
        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
    }
    break;
    case CollisionDetection_TYPE::Continuous: {
        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
    }
    break;
    case CollisionDetection_TYPE::ContinuousSpecutive: {
        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
    }
    break;
    }
}

void CRigidbody::SetFreezePosition(AXIS_TYPE _Axis, bool _Freeze)
{
    m_FreezePosition[(UINT)_Axis] = _Freeze;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;

    body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, m_FreezePosition[0]);
    body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, m_FreezePosition[1]);
    body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, m_FreezePosition[2]);
}

void CRigidbody::SetFreezeRotation(AXIS_TYPE _Axis, bool _Freeze)
{
    m_FreezeRotation[(UINT)_Axis] = _Freeze;

    if (nullptr == m_RuntimeBody)
        return;

    physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)m_RuntimeBody;

    body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, m_FreezeRotation[0]);
    body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, m_FreezeRotation[1]);
    body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, m_FreezeRotation[2]);
}

UINT CRigidbody::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Mass, sizeof(float), 1, _File);
    fwrite(&m_Drag, sizeof(float), 1, _File);
    fwrite(&m_AngularDrag, sizeof(float), 1, _File);
    fwrite(&m_bGravity, sizeof(bool), 1, _File);
    fwrite(&m_bKinematic, sizeof(bool), 1, _File);
    fwrite(&m_CollisionDetectionType, sizeof(CollisionDetection_TYPE), 1, _File);
    fwrite(&m_FreezePosition, sizeof(bool), 3, _File);
    fwrite(&m_FreezeRotation, sizeof(bool), 3, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(CollisionDetection_TYPE);
    MemoryByte += sizeof(bool) * 3;
    MemoryByte += sizeof(bool) * 3;

    return MemoryByte;
}

UINT CRigidbody::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Mass, sizeof(float), 1, _File);
    fread(&m_Drag, sizeof(float), 1, _File);
    fread(&m_AngularDrag, sizeof(float), 1, _File);
    fread(&m_bGravity, sizeof(bool), 1, _File);
    fread(&m_bKinematic, sizeof(bool), 1, _File);
    fread(&m_CollisionDetectionType, sizeof(CollisionDetection_TYPE), 1, _File);
    fread(&m_FreezePosition, sizeof(bool), 3, _File);
    fread(&m_FreezeRotation, sizeof(bool), 3, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(CollisionDetection_TYPE);
    MemoryByte += sizeof(bool) * 3;
    MemoryByte += sizeof(bool) * 3;

    return MemoryByte;
}
