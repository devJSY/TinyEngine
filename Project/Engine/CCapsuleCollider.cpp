#include "pch.h"
#include "CCapsuleCollider.h"

CCapsuleCollider::CCapsuleCollider()
    : CCollider(COMPONENT_TYPE::CAPSULECOLLIDER)
    , m_Radius(0.5f)
    , m_Height(1.f)
    , m_Direction(AXIS_TYPE::Y)
{
}

CCapsuleCollider::~CCapsuleCollider()
{
}

void CCapsuleCollider::finaltick()
{
    CCollider::finaltick();
}

void CCapsuleCollider::SaveToLevelFile(FILE* _File)
{
    CCollider::SaveToLevelFile(_File);
    fwrite(&m_Radius, sizeof(float), 1, _File);
    fwrite(&m_Height, sizeof(float), 1, _File);
    fwrite(&m_Direction, sizeof(AXIS_TYPE), 1, _File);
}

void CCapsuleCollider::LoadFromLevelFile(FILE* _File)
{
    CCollider::LoadFromLevelFile(_File);
    fread(&m_Radius, sizeof(float), 1, _File);
    fread(&m_Height, sizeof(float), 1, _File);
    fread(&m_Direction, sizeof(AXIS_TYPE), 1, _File);
}
