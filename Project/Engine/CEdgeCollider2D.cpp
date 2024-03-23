#include "pch.h"
#include "CEdgeCollider2D.h"

CEdgeCollider2D::CEdgeCollider2D()
    : CCollider2D(COMPONENT_TYPE::EDGECOLLIDER2D)
{
}

CEdgeCollider2D::~CEdgeCollider2D()
{
}

void CEdgeCollider2D::finaltick()
{
}

void CEdgeCollider2D::SaveToLevelFile(FILE* _File)
{
}

void CEdgeCollider2D::LoadFromLevelFile(FILE* _File)
{
}
