#include "pch.h"
#include "CBoxCollider2D.h"

#include "components.h"

CBoxCollider2D::CBoxCollider2D()
    : CCollider2D(COMPONENT_TYPE::BOXCOLLIDER2D)
    , m_Size(Vec2(0.5f, 0.5f))
{
}

CBoxCollider2D::~CBoxCollider2D()
{
}

void CBoxCollider2D::finaltick()
{
    Vec3 pos = Transform()->GetWorldPos();
    pos.x += m_Offset.x;
    pos.y += m_Offset.y;
    Vec3 scale = Transform()->GetRelativeScale();
    scale.x *= m_Size.x * 2.f;
    scale.y *= m_Size.y * 2.f;
    GamePlayStatic::DrawDebugRect(pos, scale, Transform()->GetRelativeRotation(), Vec3(0.f, 1.f, 0.f), false);
}

void CBoxCollider2D::SaveToLevelFile(FILE* _File)
{
    CCollider2D::SaveToLevelFile(_File);
    fwrite(&m_Size, sizeof(Vec2), 1, _File);
}

void CBoxCollider2D::LoadFromLevelFile(FILE* _File)
{
    CCollider2D::LoadFromLevelFile(_File);
    fread(&m_Size, sizeof(Vec2), 1, _File);
}
