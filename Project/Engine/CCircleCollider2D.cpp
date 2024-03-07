#include "pch.h"
#include "CCircleCollider2D.h"

CCircleCollider2D::CCircleCollider2D()
    : CComponent(COMPONENT_TYPE::CIRCLECOLLIDER2D)
{
}

CCircleCollider2D::~CCircleCollider2D()
{
}

void CCircleCollider2D::finaltick()
{
}

void CCircleCollider2D::SaveToLevelFile(FILE* _File)
{
}

void CCircleCollider2D::LoadFromLevelFile(FILE* _File)
{
}
