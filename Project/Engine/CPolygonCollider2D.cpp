#include "pch.h"
#include "CPolygonCollider2D.h"

#include "components.h"

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
    CCollider2D::finaltick();

    if (!m_Points.empty())
    {
        Vec3 sacle = Transform()->GetWorldScale();

        Matrix matWorld = Transform()->GetWorldMat();
        Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.0f);
        Matrix matScale = XMMatrixScaling(sacle.x, sacle.y, 1.f);

        Matrix matInvScale = XMMatrixScaling(1.f / sacle.x, 1.f / sacle.y, 1.f / sacle.z);

        Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

        GamePlayStatic::DrawDebugPolygon(matScale * matTranslation * matInvScale * matWorld, color, m_Points, false);
    }
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
