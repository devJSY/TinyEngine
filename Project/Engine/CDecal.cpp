#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

#include "CRenderMgr.h"

CDecal::CDecal()
    : CRenderComponent(COMPONENT_TYPE::DECAL)
    , m_DecalColorTex(nullptr)
    , m_DecalNormalTex(nullptr)
    , m_bAsEmissive(false)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"));
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
    GamePlayStatic::DrawDebugBox(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 0.f), true);
}

void CDecal::UpdateData()
{
    Transform()->UpdateData();

    Matrix matInv = Transform()->GetWorldMat().Invert();
    GetMaterial()->SetScalarParam(MAT_0, matInv);
    GetMaterial()->SetScalarParam(INT_0, m_bAsEmissive);
    GetMaterial()->SetTexParam(TEX_4, m_DecalColorTex);
    GetMaterial()->SetTexParam(TEX_5, m_DecalNormalTex);

    GetMaterial()->UpdateData();
}

void CDecal::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial())
        return;

    UpdateData();

    GetMesh()->render();
}

void CDecal::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == GetMesh() || nullptr == _mtrl)
        return;

    Transform()->UpdateData();
    _mtrl->UpdateData();

    GetMesh()->render();
}

void CDecal::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);
    SaveAssetRef(m_DecalColorTex, _File);
    SaveAssetRef(m_DecalNormalTex, _File);
    fwrite(&m_bAsEmissive, sizeof(int), 1, _File);
}

void CDecal::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);
    LoadAssetRef(m_DecalColorTex, _File);
    LoadAssetRef(m_DecalNormalTex, _File);
    fread(&m_bAsEmissive, sizeof(int), 1, _File);
}
