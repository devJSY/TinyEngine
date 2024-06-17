#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

#include "CRenderMgr.h"

CDecal::CDecal()
    : CRenderComponent(COMPONENT_TYPE::DECAL)
    , m_DecalColorTex(nullptr)
    , m_DecalNormalTex(nullptr)
    , m_bInvertNormalY(false)
    , m_bAsEmissive(false)
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
    GetMaterial(0)->SetScalarParam(INT_0, m_bAsEmissive);
    GetMaterial(0)->SetScalarParam(INT_1, m_bInvertNormalY);
    GetMaterial(0)->SetTexParam(TEX_4, m_DecalColorTex);
    GetMaterial(0)->SetTexParam(TEX_5, m_DecalNormalTex);

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

void CDecal::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);
    SaveAssetRef(m_DecalColorTex, _File);
    SaveAssetRef(m_DecalNormalTex, _File);
    fwrite(&m_bInvertNormalY, sizeof(int), 1, _File);
    fwrite(&m_bAsEmissive, sizeof(int), 1, _File);
}

void CDecal::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);
    LoadAssetRef(m_DecalColorTex, _File);
    LoadAssetRef(m_DecalNormalTex, _File);
    fread(&m_bInvertNormalY, sizeof(int), 1, _File);
    fread(&m_bAsEmissive, sizeof(int), 1, _File);
}
