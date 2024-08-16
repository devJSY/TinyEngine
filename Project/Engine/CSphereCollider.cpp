#include "pch.h"
#include "CSphereCollider.h"
#include "components.h"

CSphereCollider::CSphereCollider()
    : CCollider(COMPONENT_TYPE::SPHERECOLLIDER)
    , m_Radius(0.5f)
{
}

CSphereCollider::~CSphereCollider()
{
}

void CSphereCollider::begin()
{
    CCollider::begin();
}

void CSphereCollider::finaltick()
{
    CCollider::finaltick();

    // 콜라이더 비활성화 상태에서는 렌더링 X
    if (!m_bEnabled)
        return;

    Vec3 scale = Transform()->GetWorldScale();
    const float WorldRatio = Transform()->GetWorldRatio();

    Matrix matPhysXScale = XMMatrixScaling(scale.x * m_Radius * 2.f, scale.y * m_Radius * 2.f, scale.z * m_Radius * 2.f);
    Matrix matWorldScaleInv = XMMatrixScaling(1.f / scale.x, 1.f / scale.y, 1.f / scale.z);
    Matrix matCenterTrans = XMMatrixTranslation(WorldRatio * m_Center.x, WorldRatio * m_Center.y, WorldRatio * m_Center.z);
    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    // PhysX SRT * Transform SRT
    GamePlayStatic::DrawDebugSphere(matPhysXScale * matCenterTrans * matWorldScaleInv * Transform()->GetWorldMat(), color, true);
}

void CSphereCollider::SetRadius(float _Radius)
{
    if (m_Radius == _Radius)
        return;

    m_Radius = _Radius;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

UINT CSphereCollider::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider::SaveToLevelFile(_File);
    fwrite(&m_Radius, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CSphereCollider::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider::LoadFromLevelFile(_File);
    fread(&m_Radius, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}
