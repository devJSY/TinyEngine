#include "pch.h"
#include "CBoxCollider2D.h"

#include "components.h"

CBoxCollider2D::CBoxCollider2D()
    : CComponent(COMPONENT_TYPE::BOXCOLLIDER2D)
    , m_RuntimeFixture(nullptr)
    , m_Offset(Vec2(0.f, 0.f))
    , m_Size(Vec2(0.5f, 0.5f))
    , m_Density(1.f)
    , m_Friction(0.5f)
    , m_Restitution(0.f)
    , m_RestitutionThreshold(0.5f)
{
}

CBoxCollider2D::~CBoxCollider2D()
{
}

void CBoxCollider2D::finaltick()
{
    Vec3 pos = Transform()->GetWorldPos();
    pos.x += m_Offset.x;
    pos.y += m_Offset.y;
    Vec3 scale = Transform()->GetRelativeScale();
    scale.x *= m_Size.x * 2.f;
    scale.y *= m_Size.y * 2.f;
    GamePlayStatic::DrawDebugRect(pos, scale, Transform()->GetRelativeRotation(), Vec3(0.f, 1.f, 0.f), false);
}

void CBoxCollider2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Offset, sizeof(Vec2), 1, _File);
    fwrite(&m_Size, sizeof(Vec2), 1, _File);
    fwrite(&m_Density, sizeof(float), 1, _File);
    fwrite(&m_Friction, sizeof(float), 1, _File);
    fwrite(&m_Restitution, sizeof(float), 1, _File);
    fwrite(&m_RestitutionThreshold, sizeof(float), 1, _File);
}

void CBoxCollider2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Offset, sizeof(Vec2), 1, _File);
    fread(&m_Size, sizeof(Vec2), 1, _File);
    fread(&m_Density, sizeof(float), 1, _File);
    fread(&m_Friction, sizeof(float), 1, _File);
    fread(&m_Restitution, sizeof(float), 1, _File);
    fread(&m_RestitutionThreshold, sizeof(float), 1, _File);
}
