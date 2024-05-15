#include "pch.h"
#include "CCharacterController.h"
#include "CTransform.h"

CCharacterController::CCharacterController()
    : CCollider(COMPONENT_TYPE::CHARACTERCONTROLLER)
    , m_SlopeLimit(45.f)
    , m_StepOffset(0.3f)
    , m_SkinWidth(0.08f)
    , m_MinMoveDistance(0.001f)
    , m_Radius(0.5f)
    , m_Height(2.f)
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
    CCollider::finaltick();

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

    // PhysX SRT * Transform SRT
    GamePlayStatic::DrawDebugCapsule(matPhysXScale * matCenterTrans * matWorldScaleInv * Transform()->GetWorldMat(), 1.f, 1.f, AXIS_TYPE::Y, color,
                                     true);
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
