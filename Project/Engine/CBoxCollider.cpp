#include "pch.h"
#include "CBoxCollider.h"
#include "components.h"

CBoxCollider::CBoxCollider()
    : CCollider(COMPONENT_TYPE::BOXCOLLIDER)
    , m_Size(Vec3(1.f, 1.f, 1.f))
{
}

CBoxCollider::~CBoxCollider()
{
}

void CBoxCollider::begin()
{
    CCollider::begin();
}

void CBoxCollider::finaltick()
{
    CCollider::finaltick();

    // 콜라이더 비활성화 상태에서는 렌더링 X
    if (!m_bEnabled)
        return; 

    Vec3 scale = Transform()->GetWorldScale();

    Matrix matPhysXScale = XMMatrixScaling(scale.x * m_Size.x, scale.y * m_Size.y, scale.z * m_Size.z);
    Matrix matWorldScaleInv = XMMatrixScaling(1.f / scale.x, 1.f / scale.y, 1.f / scale.z);
    Matrix matCenterTrans = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    // PhysX SRT * Transform SRT
    GamePlayStatic::DrawDebugBox(matPhysXScale * matCenterTrans * matWorldScaleInv * Transform()->GetWorldMat(), color, true);
}

void CBoxCollider::SetSize(Vec3 _size)
{
    m_Size = _size;
    GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
}

void CBoxCollider::SaveToLevelFile(FILE* _File)
{
    CCollider::SaveToLevelFile(_File);
    fwrite(&m_Size, sizeof(Vec3), 1, _File);
}

void CBoxCollider::LoadFromLevelFile(FILE* _File)
{
    CCollider::LoadFromLevelFile(_File);
    fread(&m_Size, sizeof(Vec3), 1, _File);
}
