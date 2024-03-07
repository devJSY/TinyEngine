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
