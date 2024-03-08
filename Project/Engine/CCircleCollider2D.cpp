#include "pch.h"
#include "CCircleCollider2D.h"

#include "components.h"

CCircleCollider2D::CCircleCollider2D()
    : CCollider2D(COMPONENT_TYPE::CIRCLECOLLIDER2D)
    , m_Radius(1.f)

{
}

CCircleCollider2D::~CCircleCollider2D()
{
}

void CCircleCollider2D::finaltick()
{
    Vec3 pos = Transform()->GetWorldPos();
    pos.x += m_Offset.x;
    pos.y += m_Offset.y;
    GamePlayStatic::DrawDebugCircle(pos, Transform()->GetRelativeScale().x * m_Radius, Vec3(0.f, 1.f, 0.f), false);
}

void CCircleCollider2D::SaveToLevelFile(FILE* _File)
{
    CCollider2D::SaveToLevelFile(_File);
    fwrite(&m_Radius, sizeof(float), 1, _File);
}

void CCircleCollider2D::LoadFromLevelFile(FILE* _File)
{
    CCollider2D::LoadFromLevelFile(_File);
    fread(&m_Radius, sizeof(float), 1, _File);
}
