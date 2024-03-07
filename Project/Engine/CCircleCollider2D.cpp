#include "pch.h"
#include "CCircleCollider2D.h"

#include "components.h"

CCircleCollider2D::CCircleCollider2D()
    : CComponent(COMPONENT_TYPE::CIRCLECOLLIDER2D)
    , m_RuntimeFixture(nullptr)
    , m_Offset(Vec2(0.f, 0.f))
    , m_Radius(1.f)
    , m_Density(1.f)
    , m_Friction(0.5f)
    , m_Restitution(0.f)
    , m_RestitutionThreshold(0.5f)
{
}

CCircleCollider2D::~CCircleCollider2D()
{
}

void CCircleCollider2D::finaltick()
{
    Vec3 pos = Transform()->GetWorldPos();
    pos.x += m_Offset.x;
    pos.y += m_Offset.y;
    GamePlayStatic::DrawDebugCircle(pos, m_Radius, Vec3(0.f, 1.f, 0.f), false);
}

void CCircleCollider2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Offset, sizeof(Vec2), 1, _File);
    fwrite(&m_Radius, sizeof(float), 1, _File);
    fwrite(&m_Density, sizeof(float), 1, _File);
    fwrite(&m_Friction, sizeof(float), 1, _File);
    fwrite(&m_Restitution, sizeof(float), 1, _File);
    fwrite(&m_RestitutionThreshold, sizeof(float), 1, _File);
}

void CCircleCollider2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Offset, sizeof(Vec2), 1, _File);
    fread(&m_Radius, sizeof(float), 1, _File);
    fread(&m_Density, sizeof(float), 1, _File);
    fread(&m_Friction, sizeof(float), 1, _File);
    fread(&m_Restitution, sizeof(float), 1, _File);
    fread(&m_RestitutionThreshold, sizeof(float), 1, _File);
}
