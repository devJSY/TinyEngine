#include "pch.h"
#include "CCollider2D.h"

CCollider2D::CCollider2D(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_RuntimeFixture(nullptr)
    , m_Offset(Vec2(0.f, 0.f))
    , m_Density(1.f)
    , m_Friction(0.5f)
    , m_Restitution(0.f)
    , m_RestitutionThreshold(0.5f)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Offset, sizeof(Vec2), 1, _File);
    fwrite(&m_Density, sizeof(float), 1, _File);
    fwrite(&m_Friction, sizeof(float), 1, _File);
    fwrite(&m_Restitution, sizeof(float), 1, _File);
    fwrite(&m_RestitutionThreshold, sizeof(float), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Offset, sizeof(Vec2), 1, _File);
    fread(&m_Density, sizeof(float), 1, _File);
    fread(&m_Friction, sizeof(float), 1, _File);
    fread(&m_Restitution, sizeof(float), 1, _File);
    fread(&m_RestitutionThreshold, sizeof(float), 1, _File);
}
