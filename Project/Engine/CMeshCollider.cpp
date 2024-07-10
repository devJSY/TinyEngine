#include "pch.h"
#include "CMeshCollider.h"

#include "CTransform.h"
#include "CMeshRender.h"

CMeshCollider::CMeshCollider()
    : CCollider(COMPONENT_TYPE::MESHCOLLIDER)
    , m_bConvex(false)
    , m_Mesh(nullptr)
{
}

CMeshCollider::~CMeshCollider()
{
}

void CMeshCollider::begin()
{
    CCollider::begin();
}

void CMeshCollider::finaltick()
{
    CCollider::finaltick();

    // 콜라이더 비활성화 상태에서는 렌더링 X
    if (!m_bEnabled)
        return;

    if (nullptr != m_Mesh)
    {
        GamePlayStatic::DrawDebugMesh(Transform()->GetWorldMat(), m_Mesh, Vec3(0.f, 1.f, 0.f), true);
    }
}

void CMeshCollider::SaveToLevelFile(FILE* _File)
{
    CCollider::SaveToLevelFile(_File);
    fwrite(&m_bConvex, 1, sizeof(bool), _File);
    SaveAssetRef(m_Mesh, _File);
}

void CMeshCollider::LoadFromLevelFile(FILE* _File)
{
    CCollider::LoadFromLevelFile(_File);
    fread(&m_bConvex, 1, sizeof(bool), _File);
    LoadAssetRef(m_Mesh, _File);
}
