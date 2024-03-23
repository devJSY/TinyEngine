#include "pch.h"
#include "CPolygonCollider2D.h"

CPolygonCollider2D::CPolygonCollider2D()
    : CCollider2D(COMPONENT_TYPE::POLYGONCOLLIDER2D)
{
}

CPolygonCollider2D::~CPolygonCollider2D()
{
}

void CPolygonCollider2D::finaltick()
{
}

void CPolygonCollider2D::SaveToLevelFile(FILE* _File)
{
}

void CPolygonCollider2D::LoadFromLevelFile(FILE* _File)
{
}
