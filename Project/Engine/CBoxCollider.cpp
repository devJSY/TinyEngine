#include "pch.h"
#include "CBoxCollider.h"

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
