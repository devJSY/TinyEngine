#include "pch.h"
#include "CCharacterController.h"
#include "physx\\PxPhysicsAPI.h"

#include "CPhysicsMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CScript.h"
#include "CLevel.h"

CCharacterController::CCharacterController()
    : CCollider(COMPONENT_TYPE::CHARACTERCONTROLLER)
    , m_SlopeLimit(45.f)
    , m_StepOffset(0.3f)
    , m_SkinWidth(0.08f)
    , m_MinMoveDistance(0.f)
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
    // 트랜스폼의 스케일이 변경되었다면 재생성
    Vec3 TransformWorldScale = Transform()->GetTransformWorldScale();
    if ((m_PrevScale - TransformWorldScale).Length() > 1e-3f)
    {
        GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
    }

    m_PrevScale = TransformWorldScale;

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

    // 콜라이더 비활성화 상태에서는 렌더링 X
    if (!m_bEnabled)
        return;

    // Play, Simulate State에서 m_RuntimeShape이 존재하지 않는 경우 렌더링 X
    if (nullptr == m_RuntimeShape)
    {
        LEVEL_STATE CurLevelState = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();
        if (CurLevelState == LEVEL_STATE::PLAY || CurLevelState == LEVEL_STATE::SIMULATE)
        {
            return;
        }
    }

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

void CCharacterController::Move(Vec3 _Motion)
{
    if (nullptr == m_RuntimeShape || _Motion == Vector3::Zero)
        return;

    int LayerIdx = GetOwner()->GetLayerIdx();
    physx::PxFilterData filterData;
    filterData.word0 = (1 << LayerIdx);                                     // 해당 오브젝트의 레이어 번호
    filterData.word1 = CPhysicsMgr::GetInst()->GetCollisionLayer(LayerIdx); // 필터링을 적용할 테이블

    physx::PxControllerCollisionFlags MoveFlags =
        ((physx::PxController*)m_RuntimeShape)->move(_Motion, m_MinMoveDistance, m_MoveElapsedTime, physx::PxControllerFilters(&filterData));
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

    Transform()->SetLocalPos(Transform()->GetLocalPos() - vPosOffset);
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

Vec3 CCharacterController::GetControllerPos()
{
    if (nullptr == m_RuntimeShape)
        return Vec3();

    physx::PxController* PxCharacterController = (physx::PxController*)m_RuntimeShape;
    Vec3 pos = physx::toVec3(PxCharacterController->getPosition());
    float PPM = CPhysicsMgr::GetInst()->GetPPM();
    pos *= PPM;

    return pos;
}

Vec3 CCharacterController::GetFootPos()
{
    if (nullptr == m_RuntimeShape)
        return Vec3();

    physx::PxController* PxCharacterController = (physx::PxController*)m_RuntimeShape;
    Vec3 FootPos = physx::toVec3(PxCharacterController->getFootPosition());
    float PPM = CPhysicsMgr::GetInst()->GetPPM();
    FootPos *= PPM;

    return FootPos;
}

UINT CCharacterController::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider::SaveToLevelFile(_File);

    fwrite(&m_SlopeLimit, sizeof(float), 1, _File);
    fwrite(&m_StepOffset, sizeof(float), 1, _File);
    fwrite(&m_SkinWidth, sizeof(float), 1, _File);
    fwrite(&m_MinMoveDistance, sizeof(float), 1, _File);
    fwrite(&m_Radius, sizeof(float), 1, _File);
    fwrite(&m_Height, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CCharacterController::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider::LoadFromLevelFile(_File);

    fread(&m_SlopeLimit, sizeof(float), 1, _File);
    fread(&m_StepOffset, sizeof(float), 1, _File);
    fread(&m_SkinWidth, sizeof(float), 1, _File);
    fread(&m_MinMoveDistance, sizeof(float), 1, _File);
    fread(&m_Radius, sizeof(float), 1, _File);
    fread(&m_Height, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
