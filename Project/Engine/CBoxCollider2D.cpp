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

void CBoxCollider2D::begin()
{
    CCollider2D::begin();
}

void CBoxCollider2D::finaltick()
{
    CCollider2D::finaltick();

    // 콜라이더 비활성화 상태에서는 렌더링 X
    if (!m_bEnabled)
        return;

    Vec3 scale = Transform()->GetWorldScale();

    Matrix matWorld = Transform()->GetWorldMat();
    Matrix matTranslation = XMMatrixTranslation(m_Offset.x * scale.x, m_Offset.y * scale.y, 0.0f);
    Matrix matScale = XMMatrixScaling(2.f * m_Size.x * scale.x, 2.f * m_Size.y * scale.y, 1.f);
    Matrix matInvScale = XMMatrixScaling(1.f / scale.x, 1.f / scale.y, 1.f / scale.z);

    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    GamePlayStatic::DrawDebugRect(matScale * matTranslation * matInvScale * matWorld, color, false);
}

void CBoxCollider2D::SetSize(Vec2 _size)
{
    if (m_Size == _size)
        return;

    m_Size = _size;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

UINT CBoxCollider2D::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider2D::SaveToLevelFile(_File);
    fwrite(&m_Size, sizeof(Vec2), 1, _File);

    MemoryByte += sizeof(Vec2);

    return MemoryByte;
}

UINT CBoxCollider2D::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CCollider2D::LoadFromLevelFile(_File);
    fread(&m_Size, sizeof(Vec2), 1, _File);

    MemoryByte += sizeof(Vec2);

    return MemoryByte;
}
