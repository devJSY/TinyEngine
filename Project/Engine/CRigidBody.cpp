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
    physx::PxRigidActor* body = (physx::PxRigidActor*)m_RuntimeBody;
    physx::PxTransform PxTr = body->getGlobalPose();

    Vec3 WolrdPos = Transform()->GetWorldPos();
    PxTr.p = physx::PxVec3(WolrdPos.x, WolrdPos.y, WolrdPos.z);
    body->setGlobalPose(PxTr);
}

void CRigidbody::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Mass, sizeof(float), 1, _File);
    fwrite(&m_Drag, sizeof(float), 1, _File);
    fwrite(&m_AngularDrag, sizeof(float), 1, _File);
    fwrite(&m_bGravity, sizeof(bool), 1, _File);
    fwrite(&m_bKinematic, sizeof(bool), 1, _File);
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
    fread(&m_FreezePosition, sizeof(bool), 3, _File);
    fread(&m_FreezeRotation, sizeof(bool), 3, _File);
}
