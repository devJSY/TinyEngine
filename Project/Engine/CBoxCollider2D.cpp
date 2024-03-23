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
    CCollider2D::finaltick();

    Vec3 sacle = Transform()->GetWorldScale();

    Matrix matWorld = Transform()->GetWorldMat();
    Matrix matTranslation = XMMatrixTranslation(m_Offset.x * sacle.x, m_Offset.y * sacle.y, 0.0f);
    Matrix matScale = XMMatrixScaling(2.f * m_Size.x * sacle.x, 2.f * m_Size.y * sacle.y, 1.f);
    Matrix matInvScale = XMMatrixScaling(1.f / sacle.x, 1.f / sacle.y, 1.f / sacle.z);

    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    GamePlayStatic::DrawDebugRect(matScale * matTranslation * matInvScale * matWorld, color, false);
}

void CBoxCollider2D::SetSize(Vec2 _size)
{
    m_Size = _size;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
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
