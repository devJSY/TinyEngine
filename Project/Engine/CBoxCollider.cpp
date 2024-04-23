#include "pch.h"
#include "CBoxCollider.h"
#include "components.h"

CBoxCollider::CBoxCollider()
    : CCollider(COMPONENT_TYPE::BOXCOLLIDER)
    , m_Size(Vec3(1.f, 1.f, 1.f))
{
}

CBoxCollider::~CBoxCollider()
{
}

void CBoxCollider::finaltick()
{
    CCollider::finaltick();

    Matrix matScale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
    Matrix matTrans = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
    Vec3 color = m_CollisionCount > 0 || m_TriggerCount > 0 ? Vec3(1.f, 0.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    GamePlayStatic::DrawDebugBox(matScale * matTrans * Transform()->GetWorldMat(), color, true);
}

void CBoxCollider::SaveToLevelFile(FILE* _File)
{
    CCollider::SaveToLevelFile(_File);
    fwrite(&m_Size, sizeof(Vec3), 1, _File);
}

void CBoxCollider::LoadFromLevelFile(FILE* _File)
{
    CCollider::LoadFromLevelFile(_File);
    fread(&m_Size, sizeof(Vec3), 1, _File);
}
