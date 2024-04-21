#include "pch.h"
#include "CSphereCollider.h"

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
