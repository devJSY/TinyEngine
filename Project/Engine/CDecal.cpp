#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

CDecal::CDecal()
    : CRenderComponent(COMPONENT_TYPE::DECAL)
    , m_DecalTex(nullptr)
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
    GetMaterial()->SetTexParam(TEX_0, m_DecalTex);

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
    SaveAssetRef(m_DecalTex, _File);
    fwrite(&m_bAsEmissive, sizeof(int), 1, _File);
}

void CDecal::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);
    LoadAssetRef(m_DecalTex, _File);
    fread(&m_bAsEmissive, sizeof(int), 1, _File);
}
