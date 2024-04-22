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

void CSphereCollider::finaltick()
{
    CCollider::finaltick();

    Matrix matScale = XMMatrixScaling(m_Radius * 2.f, m_Radius * 2.f, m_Radius * 2.f);
    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    GamePlayStatic::DrawDebugSphere(matScale * Transform()->GetWorldMat(), color, false);
}

void CSphereCollider::SaveToLevelFile(FILE* _File)
{
    CCollider::SaveToLevelFile(_File);
    fwrite(&m_Radius, sizeof(float), 1, _File);
}

void CSphereCollider::LoadFromLevelFile(FILE* _File)
{
    CCollider::LoadFromLevelFile(_File);
    fread(&m_Radius, sizeof(float), 1, _File);
}
