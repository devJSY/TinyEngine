#include "pch.h"
#include "CBoxCollider2D.h"

CBoxCollider2D::CBoxCollider2D()
    : CComponent(COMPONENT_TYPE::BOXCOLLIDER2D)
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
}

void CBoxCollider2D::LoadFromLevelFile(FILE* _File)
{
}
