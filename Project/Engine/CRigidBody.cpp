#include "pch.h"
#include "CRigidBody.h"
#include "physx\\PxPhysicsAPI.h"
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

    // 트랜스폼 위치 정보 업데이트
    Vec3 WorldPos = Transform()->GetWorldPos();
    Vec3 WorldRot = Transform()->GetWorldRotation();

    SimpleMath::Quaternion QuatX = SimpleMath::Quaternion::CreateFromAxisAngle(Vec3(1.f, 0.f, 0.f), WorldRot.x);
    SimpleMath::Quaternion QuatY = SimpleMath::Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), WorldRot.y);
    SimpleMath::Quaternion QuatZ = SimpleMath::Quaternion::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), WorldRot.z);
    SimpleMath::Quaternion Quat = QuatX * QuatY * QuatZ;

    physx::PxTransform PxTr = physx::PxTransform(WorldPos, physx::PxQuat(Quat.x, Quat.y, Quat.z, Quat.w));

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

void CRigidbody::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Mass, sizeof(float), 1, _File);
    fwrite(&m_Drag, sizeof(float), 1, _File);
    fwrite(&m_AngularDrag, sizeof(float), 1, _File);
    fwrite(&m_bGravity, sizeof(bool), 1, _File);
    fwrite(&m_bKinematic, sizeof(bool), 1, _File);
    fwrite(&m_CollisionDetectionType, sizeof(CollisionDetection_TYPE), 1, _File);
    fwrite(&m_FreezePosition, sizeof(bool), 3, _File);
    fwrite(&m_FreezeRotation, sizeof(bool), 3, _File);
}

void CRigidbody::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Mass, sizeof(float), 1, _File);
    fread(&m_Drag, sizeof(float), 1, _File);
    fread(&m_AngularDrag, sizeof(float), 1, _File);
    fread(&m_bGravity, sizeof(bool), 1, _File);
    fread(&m_bKinematic, sizeof(bool), 1, _File);
    fread(&m_CollisionDetectionType, sizeof(CollisionDetection_TYPE), 1, _File);
    fread(&m_FreezePosition, sizeof(bool), 3, _File);
    fread(&m_FreezeRotation, sizeof(bool), 3, _File);
}
