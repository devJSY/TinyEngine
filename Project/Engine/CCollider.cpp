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
{
}

CCollider::~CCollider()
{
}

void CCollider::begin()
{
    m_PrevScale = Transform()->GetWorldScale();
}

void CCollider::finaltick()
{
    // 스케일이 변경되었다면 재생성
    if (m_PrevScale != Transform()->GetWorldScale())
    {
        GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
    }

    m_PrevScale = Transform()->GetWorldScale();

    if (nullptr == m_RuntimeShape)
        return;

    // 트랜스폼 위치 정보 업데이트
    float PPM = CPhysicsMgr::GetInst()->GetPPM();

    Vec3 WorldPos = Transform()->GetWorldPos();
    Vec3 WorldRot = Transform()->GetWorldRotation();

    WorldPos /= PPM;

    SimpleMath::Quaternion Quat = SimpleMath::Quaternion::CreateFromAxisAngle(Vec3(1.f, 0.f, 0.f), WorldRot.x) *
                                  SimpleMath::Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), WorldRot.y) *
                                  SimpleMath::Quaternion::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), WorldRot.z);

    physx::PxTransform PxTr = physx::PxTransform(WorldPos, physx::PxQuat(Quat.x, Quat.y, Quat.z, Quat.w));

    physx::PxShape* shape = (physx::PxShape*)m_RuntimeShape;
    physx::PxRigidActor* body = shape->getActor();
    body->setGlobalPose(PxTr);

    physx::PxTransform LocalPos = shape->getLocalPose();
    LocalPos.p = m_Center;
    LocalPos.p /= PPM;
    shape->setLocalPose(LocalPos);
}

void CCollider::SetTrigger(bool _Trigger)
{
    m_bTrigger = _Trigger;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCollider::SetMaterial(Ptr<CPhysicMaterial> _Mtrl)
{
    m_Mtrl = _Mtrl;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCollider::SetEnabled(bool _bEnabled)
{
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

void CCollider::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_bTrigger, sizeof(bool), 1, _File);
    SaveAssetRef(m_Mtrl, _File);
    fwrite(&m_Center, sizeof(Vec3), 1, _File);
    fwrite(&m_bEnabled, sizeof(bool), 1, _File);
}

void CCollider::LoadFromLevelFile(FILE* _File)
{
    fread(&m_bTrigger, sizeof(bool), 1, _File);
    LoadAssetRef(m_Mtrl, _File);
    fread(&m_Center, sizeof(Vec3), 1, _File);
    fread(&m_bEnabled, sizeof(bool), 1, _File);
}
