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
    Matrix matWorld = Transform()->GetWorldMat();
    Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.0f);

    GamePlayStatic::DrawDebugRect(matTranslation * matWorld, Vec3(0.f, 1.f, 0.f), false);
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
