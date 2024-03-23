#include "pch.h"
#include "CEdgeCollider2D.h"

#include "components.h"

CEdgeCollider2D::CEdgeCollider2D()
    : CCollider2D(COMPONENT_TYPE::EDGECOLLIDER2D)
    , m_EdgeRadius(0.f)
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

void CEdgeCollider2D::SetEdgeRadius(float _Radius)
{
    m_EdgeRadius = _Radius;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::SetStartPoint(Vec2 _Point)
{
    m_StartPoint = _Point;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::SetEndPoint(Vec2 _Point)
{
    m_EndPoint = _Point;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::SetUseAdjacentStartPoint(bool _Use)
{
    m_bUseAdjacentStartPoint = _Use;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::SetUseAdjacentEndPoint(bool _Use)
{
    m_bUseAdjacentEndPoint = _Use;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::SetAdjacentStartPoint(Vec2 _Point)
{
    m_AdjacentStartPoint = _Point;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::SetAdjacentEndPoint(Vec2 _Point)
{
    m_AdjacentEndPoint = _Point;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CEdgeCollider2D::finaltick()
{
    CCollider2D::finaltick();

    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);
    Matrix matWorld = Transform()->GetWorldMat();
    Vec3 WorldScale = Transform()->GetWorldScale();
    Matrix matInvScale = XMMatrixScaling(1.f / WorldScale.x, 1.f / WorldScale.y, 1.f / WorldScale.z);

    Vec3 StartPoint = Vec3(m_StartPoint.x + m_Offset.x, m_StartPoint.y + m_Offset.y, 0.f);
    Vec3 EndPoint = Vec3(m_EndPoint.x + m_Offset.x, m_EndPoint.y + m_Offset.y, 0.f);

    StartPoint *= WorldScale;
    EndPoint *= WorldScale;

    GamePlayStatic::DrawDebugLine(matInvScale * matWorld, StartPoint, EndPoint, color, false);

    if (m_bUseAdjacentStartPoint)
    {
        Vec3 AdjacentStartPoint = Vec3(m_AdjacentStartPoint.x + m_Offset.x, m_AdjacentStartPoint.y + m_Offset.y, 0.f);
        AdjacentStartPoint *= WorldScale;
        GamePlayStatic::DrawDebugLine(matInvScale * matWorld, StartPoint, AdjacentStartPoint, Vec3(0.f, 0.63f, 0.909f), false);
    }

    if (m_bUseAdjacentEndPoint)
    {
        Vec3 AdjacentEndPoint = Vec3(m_AdjacentEndPoint.x + m_Offset.x, m_AdjacentEndPoint.y + m_Offset.y, 0.f);
        AdjacentEndPoint *= WorldScale;
        GamePlayStatic::DrawDebugLine(matInvScale * matWorld, EndPoint, AdjacentEndPoint, Vec3(0.f, 0.63f, 0.909f), false);
    }
}

void CEdgeCollider2D::SaveToLevelFile(FILE* _File)
{
    CCollider2D::SaveToLevelFile(_File);
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
    CCollider2D::LoadFromLevelFile(_File);
    fread(&m_EdgeRadius, sizeof(float), 1, _File);
    fread(&m_StartPoint, sizeof(Vec2), 1, _File);
    fread(&m_EndPoint, sizeof(Vec2), 1, _File);
    fread(&m_bUseAdjacentStartPoint, sizeof(bool), 1, _File);
    fread(&m_bUseAdjacentEndPoint, sizeof(bool), 1, _File);
    fread(&m_AdjacentEndPoint, sizeof(Vec2), 1, _File);
    fread(&m_bUseAdjacentEndPoint, sizeof(Vec2), 1, _File);
}
