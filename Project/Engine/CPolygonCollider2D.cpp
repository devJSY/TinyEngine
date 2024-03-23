#include "pch.h"
#include "CPolygonCollider2D.h"

CPolygonCollider2D::CPolygonCollider2D()
    : CCollider2D(COMPONENT_TYPE::POLYGONCOLLIDER2D)
    , m_Points{}
{
}

CPolygonCollider2D::~CPolygonCollider2D()
{
}

void CPolygonCollider2D::PointReSize(int _size)
{
    if (_size < 0)
        _size = 0;

    // ÃÖ´ë 8°³
    if (_size > 8)
        _size = 8;

    m_Points.resize(_size);
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CPolygonCollider2D::finaltick()
{
}

void CPolygonCollider2D::SaveToLevelFile(FILE* _File)
{
    int count = (int)m_Points.size();
    fwrite(&count, sizeof(int), 1, _File);
    for (size_t i = 0; i < count; i++)
    {
        fwrite(&m_Points[i], sizeof(Vec2), 1, _File);
    }
}

void CPolygonCollider2D::LoadFromLevelFile(FILE* _File)
{
    int count = 0;
    fread(&count, sizeof(int), 1, _File);
    m_Points.resize(count);
    for (size_t i = 0; i < count; i++)
    {
        fread(&m_Points[i], sizeof(Vec2), 1, _File);
    }
}
