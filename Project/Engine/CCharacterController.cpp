#include "pch.h"
#include "CCharacterController.h"
#include "physx\\PxPhysicsAPI.h"

#include "CPhysicsMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CScript.h"

CCharacterController::CCharacterController()
    : CCollider(COMPONENT_TYPE::CHARACTERCONTROLLER)
    , m_SlopeLimit(45.f)
    , m_StepOffset(0.3f)
    , m_SkinWidth(0.08f)
    , m_MinMoveDistance(0.001f)
    , m_Radius(0.5f)
    , m_Height(2.f)
    , m_MoveElapsedTime(0.f)
    , m_bGrounded(false)
{
}

CCharacterController::~CCharacterController()
{
}

void CCharacterController::begin()
{
    CCollider::begin();
}

void CCharacterController::finaltick()
{
    // 스케일이 변경되었다면 재생성
    if (m_PrevScale != Transform()->GetWorldScale())
    {
        GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
    }

    m_PrevScale = Transform()->GetWorldScale();

    if (nullptr != m_RuntimeShape)
    {
        float PPM = CPhysicsMgr::GetInst()->GetPPM();
        Vec3 WorldPos = Transform()->GetWorldPos();
        WorldPos += m_Center;
        WorldPos /= PPM;

        physx::PxController* PxCharacterController = (physx::PxController*)m_RuntimeShape;
        PxCharacterController->setPosition(physx::PxVec3d(WorldPos.x, WorldPos.y, WorldPos.z));
    }

    m_MoveElapsedTime += DT;

    Vec3 worldpos = Transform()->GetWorldPos();
    Vec3 scale = Transform()->GetWorldScale();

    float HalfHeight = m_Height / 2.f - m_Radius;

    float RadiusScale = 1.f;
    float HeightScale = 1.f;

    RadiusScale = scale.x > scale.z ? scale.x : scale.z;
    HeightScale = scale.y;
    Matrix matPhysXScale = XMMatrixScaling(RadiusScale * m_Radius, HeightScale * HalfHeight, RadiusScale * m_Radius);

    Matrix matCenterTrans = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
    Matrix matWorldScaleInv = XMMatrixScaling(1.f / scale.x, 1.f / scale.y, 1.f / scale.z);
    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Matrix matWorldTrans = XMMatrixTranslation(worldpos.x, worldpos.y, worldpos.z);
    Matrix matWorldScale = XMMatrixScaling(scale.x, scale.y, scale.z);

    // PhysX SRT * Transform ST
    GamePlayStatic::DrawDebugCapsule(matPhysXScale * matCenterTrans * matWorldScaleInv * matWorldScale * matWorldTrans, 1.f, 1.f, AXIS_TYPE::Y, color,
                                     true);
}

Vec3 CCharacterController::Move(Vec3 _Motion)
{
    if (nullptr == m_RuntimeShape)
        return Vec3();

    physx::PxControllerCollisionFlags MoveFlags =
        ((physx::PxController*)m_RuntimeShape)->move(_Motion, m_MinMoveDistance, m_MoveElapsedTime, physx::PxControllerFilters());
    m_MoveElapsedTime = 0.f;

    m_bGrounded = physx::PxControllerCollisionFlag::eCOLLISION_DOWN & MoveFlags;

    // 트랜스폼 업데이트
    float PPM = CPhysicsMgr::GetInst()->GetPPM();
    physx::PxController* PxCharacterController = (physx::PxController*)m_RuntimeShape;
    physx::PxVec3d PxPos = PxCharacterController->getPosition();
    Vec3 vPosOffset = Vec3((float)PxPos.x, (float)PxPos.y, (float)PxPos.z);
    vPosOffset -= m_Center / PPM;
    vPosOffset *= PPM;
    vPosOffset = Transform()->GetWorldPos() - vPosOffset;

    Transform()->SetRelativePos(Transform()->GetRelativePos() - vPosOffset);
    return -vPosOffset;
}

void CCharacterController::SetSlopeLimit(float _Limit)
{
    m_SlopeLimit = _Limit;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCharacterController::SetStepOffset(float _Offset)
{
    m_StepOffset = _Offset;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCharacterController::SetSkinWidth(float _Width)
{
    m_SkinWidth = _Width;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCharacterController::SetMinMoveDistance(float _Dist)
{
    m_MinMoveDistance = _Dist;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCharacterController::SetRadius(float _Radius)
{
    m_Radius = _Radius;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCharacterController::SetHeight(float _Height)
{
    m_Height = _Height;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CCharacterController::SaveToLevelFile(FILE* _File)
{
    CCollider::SaveToLevelFile(_File);

    fwrite(&m_SlopeLimit, sizeof(float), 1, _File);
    fwrite(&m_StepOffset, sizeof(float), 1, _File);
    fwrite(&m_SkinWidth, sizeof(float), 1, _File);
    fwrite(&m_MinMoveDistance, sizeof(float), 1, _File);
    fwrite(&m_Radius, sizeof(float), 1, _File);
    fwrite(&m_Height, sizeof(float), 1, _File);
}

void CCharacterController::LoadFromLevelFile(FILE* _File)
{
    CCollider::LoadFromLevelFile(_File);

    fread(&m_SlopeLimit, sizeof(float), 1, _File);
    fread(&m_StepOffset, sizeof(float), 1, _File);
    fread(&m_SkinWidth, sizeof(float), 1, _File);
    fread(&m_MinMoveDistance, sizeof(float), 1, _File);
    fread(&m_Radius, sizeof(float), 1, _File);
    fread(&m_Height, sizeof(float), 1, _File);
}
