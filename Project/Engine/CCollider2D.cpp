#include "pch.h"
#include "CCollider2D.h"
#include <box2d\\b2_fixture.h>

CCollider2D::CCollider2D(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_RuntimeFixture(nullptr)
    , m_bTrigger(false)
    , m_Offset(Vec2(0.f, 0.f))
    , m_Density(1.f)
    , m_Friction(0.5f)
    , m_Bounciness(0.f)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::SetTrigger(bool _trigger)
{
    m_bTrigger = _trigger;

    if (nullptr != m_RuntimeFixture)
        ((b2Fixture*)m_RuntimeFixture)->SetSensor(m_bTrigger);
}

void CCollider2D::SetDensity(float _Density)
{
    m_Density = _Density;

    if (nullptr != m_RuntimeFixture)
        ((b2Fixture*)m_RuntimeFixture)->SetDensity(m_Density);
}

void CCollider2D::SetFriction(float _Friction)
{
    m_Friction = _Friction;

    if (nullptr != m_RuntimeFixture)
        ((b2Fixture*)m_RuntimeFixture)->SetFriction(m_Friction);
}

void CCollider2D::SetBounciness(float _Bounciness)
{
    m_Bounciness = _Bounciness;

    if (nullptr != m_RuntimeFixture)
        ((b2Fixture*)m_RuntimeFixture)->SetRestitution(m_Bounciness);
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Offset, sizeof(Vec2), 1, _File);
    fwrite(&m_bTrigger, sizeof(bool), 1, _File);
    fwrite(&m_Density, sizeof(float), 1, _File);
    fwrite(&m_Friction, sizeof(float), 1, _File);
    fwrite(&m_Bounciness, sizeof(float), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Offset, sizeof(Vec2), 1, _File);
    fread(&m_bTrigger, sizeof(bool), 1, _File);
    fread(&m_Density, sizeof(float), 1, _File);
    fread(&m_Friction, sizeof(float), 1, _File);
    fread(&m_Bounciness, sizeof(float), 1, _File);
}
