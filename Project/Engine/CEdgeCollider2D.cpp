#include "pch.h"
#include "CEdgeCollider2D.h"

CEdgeCollider2D::CEdgeCollider2D()
    : CCollider2D(COMPONENT_TYPE::EDGECOLLIDER2D)
    , m_EdgeRadius(1.f)
    , m_StartPoint(Vec2())
    , m_EndPoint(Vec2())
    , m_bUseAdjacentStartPoint(false)
    , m_bUseAdjacentEndPoint(false)
    , m_AdjacentStartPoint(Vec2())
    , m_AdjacentEndPoint(Vec2())
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
    fwrite(&m_EdgeRadius, sizeof(float), 1, _File);
    fwrite(&m_StartPoint, sizeof(Vec2), 1, _File);
    fwrite(&m_EndPoint, sizeof(Vec2), 1, _File);
    fwrite(&m_bUseAdjacentStartPoint, sizeof(bool), 1, _File);
    fwrite(&m_bUseAdjacentEndPoint, sizeof(bool), 1, _File);
    fwrite(&m_AdjacentEndPoint, sizeof(Vec2), 1, _File);
    fwrite(&m_bUseAdjacentEndPoint, sizeof(Vec2), 1, _File);
}

void CEdgeCollider2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_EdgeRadius, sizeof(float), 1, _File);
    fread(&m_StartPoint, sizeof(Vec2), 1, _File);
    fread(&m_EndPoint, sizeof(Vec2), 1, _File);
    fread(&m_bUseAdjacentStartPoint, sizeof(bool), 1, _File);
    fread(&m_bUseAdjacentEndPoint, sizeof(bool), 1, _File);
    fread(&m_AdjacentEndPoint, sizeof(Vec2), 1, _File);
    fread(&m_bUseAdjacentEndPoint, sizeof(Vec2), 1, _File);
}
