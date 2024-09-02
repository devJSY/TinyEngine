#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

#include "CRenderMgr.h"

CDecal::CDecal()
    : CRenderComponent(COMPONENT_TYPE::DECAL)
    , m_DecalAlbedoTex(nullptr)
    , m_DecalMRATex(nullptr)
    , m_DecalNormalTex(nullptr)
    , m_DecalEmissiveTex(nullptr)
    , m_Emissive(Vec4())
    , m_bInvertNormalY(false)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"), 0);

    SetFrustumCheck(false);
    SetCastShadow(false);
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
    CRenderComponent::finaltick();

    GamePlayStatic::DrawDebugBox(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 0.f), true);
}

void CDecal::UpdateData()
{
    Transform()->UpdateData();

    Matrix matInv = Transform()->GetWorldInvMat();
    GetMaterial(0)->SetScalarParam(MAT_0, matInv);
    GetMaterial(0)->SetScalarParam(INT_0, m_bInvertNormalY);
    GetMaterial(0)->SetScalarParam(VEC4_0, m_Emissive);
    GetMaterial(0)->SetTexParam(TEX_4, m_DecalAlbedoTex);
    GetMaterial(0)->SetTexParam(TEX_5, m_DecalMRATex);
    GetMaterial(0)->SetTexParam(TEX_6, m_DecalNormalTex);
    GetMaterial(0)->SetTexParam(TEX_7, m_DecalEmissiveTex);

    GetMaterial(0)->UpdateData();
}

void CDecal::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial(0))
        return;

    UpdateData();

    GetMesh()->render(0);
}

void CDecal::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == GetMesh() || nullptr == _mtrl)
        return;

    Transform()->UpdateData();
    _mtrl->UpdateData();

    GetMesh()->render(0);
}

void CDecal::render(UINT _Subset)
{
    render();
}

UINT CDecal::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CRenderComponent::SaveToLevelFile(_File);
    MemoryByte += SaveAssetRef(m_DecalAlbedoTex, _File);
    MemoryByte += SaveAssetRef(m_DecalMRATex, _File);
    MemoryByte += SaveAssetRef(m_DecalNormalTex, _File);
    MemoryByte += SaveAssetRef(m_DecalEmissiveTex, _File);
    fwrite(&m_Emissive, sizeof(Vec4), 1, _File);
    fwrite(&m_bInvertNormalY, sizeof(int), 1, _File);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(int);

    return MemoryByte;
}

UINT CDecal::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CRenderComponent::LoadFromLevelFile(_File);
    MemoryByte += LoadAssetRef(m_DecalAlbedoTex, _File);
    MemoryByte += LoadAssetRef(m_DecalMRATex, _File);
    MemoryByte += LoadAssetRef(m_DecalNormalTex, _File);
    MemoryByte += LoadAssetRef(m_DecalEmissiveTex, _File);
    fread(&m_Emissive, sizeof(Vec4), 1, _File);
    fread(&m_bInvertNormalY, sizeof(int), 1, _File);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(int);

    return MemoryByte;
}
