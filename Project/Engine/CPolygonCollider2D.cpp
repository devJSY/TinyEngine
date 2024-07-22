#include "pch.h"
#include "CPolygonCollider2D.h"

#include "components.h"
#include <box2d\\b2_fixture.h>

CPolygonCollider2D::CPolygonCollider2D()
    : CCollider2D(COMPONENT_TYPE::POLYGONCOLLIDER2D)
    , m_Points{}
{
    // 기본 도형 오각형
    m_Points.resize(5);
    float fTheta = 0;
    for (UINT i = 0; i < 5; i++)
    {
        fTheta = (XM_2PI / 5) * i;
        m_Points[i] = Vec2(cosf(fTheta), sinf(fTheta));
    }
}

CPolygonCollider2D::~CPolygonCollider2D()
{
}

void CPolygonCollider2D::AddPoint(Vec2 _Point)
{
    m_Points.push_back(_Point);
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CPolygonCollider2D::SetPoint(int _Idx, Vec2 _Point)
{
    if (_Idx >= m_Points.size())
        return;

    m_Points[_Idx] = _Point;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CPolygonCollider2D::PointReSize(int _size)
{
    if (_size < 0)
        _size = 0;

    // 최대 8개
    if (_size > 8)
        _size = 8;

    m_Points.resize(_size);
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CPolygonCollider2D::begin()
{
    CCollider2D::begin();
}

void CPolygonCollider2D::finaltick()
{
    CCollider2D::finaltick();

    // 콜라이더 비활성화 상태에서는 렌더링 X
    if (!m_bEnabled)
        return;

    if (!m_Points.empty())
    {
        Matrix matWorld = Transform()->GetWorldMat();
        Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.0f);

        Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

        GamePlayStatic::DrawDebugPolygon(matTranslation * matWorld, color, m_Points, false);
    }
}

UINT CPolygonCollider2D::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider2D::SaveToLevelFile(_File);
    int count = (int)m_Points.size();
    fwrite(&count, sizeof(int), 1, _File);
    for (size_t i = 0; i < count; i++)
    {
        fwrite(&m_Points[i], sizeof(Vec2), 1, _File);
    }

    MemoryByte += sizeof(int);
    MemoryByte += count * sizeof(Vec2);

    return MemoryByte;
}

UINT CPolygonCollider2D::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider2D::LoadFromLevelFile(_File);
    int count = 0;
    fread(&count, sizeof(int), 1, _File);
    m_Points.resize(count);
    for (size_t i = 0; i < count; i++)
    {
        fread(&m_Points[i], sizeof(Vec2), 1, _File);
    }

    MemoryByte += sizeof(int);
    MemoryByte += count * sizeof(Vec2);

    return MemoryByte;
}
