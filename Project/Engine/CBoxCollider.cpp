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

void CBoxCollider::finaltick()
{
    Vec3 WorldScale = Transform()->GetWorldScale();
    WorldScale *= m_Size; 
    Matrix matScale = XMMatrixScaling(WorldScale.x, WorldScale.y, WorldScale.z);
    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    GamePlayStatic::DrawDebugBox(matScale * m_RenderMatrix, color, false);
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
