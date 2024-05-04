#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"

#include "CDevice.h"
#include "CAssetMgr.h"

CSkyBox::CSkyBox()
    : CRenderComponent(COMPONENT_TYPE::SKYBOX)
    , m_Type(SKYBOX_TYPE::moonless)
    , m_Shape(SKYBOX_SHAPE::BOX)
{
    SetType(m_Type);
    SetShape(m_Shape);
}

CSkyBox::~CSkyBox()
{
    CTexture::Clear(17);
    CTexture::Clear(18);
    CTexture::Clear(19);
}

void CSkyBox::finaltick()
{
    if (GetOwner()->IsDead())
    {
        CTexture::Clear(17);
        CTexture::Clear(18);
        CTexture::Clear(19);
    }
}

void CSkyBox::UpdateData()
{
    if (nullptr != m_BrdfTex)
        m_BrdfTex->UpdateData(17);
    if (nullptr != m_DiffuseTex)
        m_DiffuseTex->UpdateData(18);
    if (nullptr != m_SpecularTex)
        m_SpecularTex->UpdateData(19);
}

void CSkyBox::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial() || GetOwner()->IsDead())
        return;
    
    UpdateData();

    Transform()->UpdateData();

    GetMaterial()->SetTexParam(TEXCUBE_0, m_EnvTex);

    GetMaterial()->UpdateData();

    GetMesh()->render();
}

void CSkyBox::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == GetMesh() || nullptr == _mtrl)
        return;

    Transform()->UpdateData();

    _mtrl->SetTexParam(TEXCUBE_0, m_EnvTex);

    _mtrl->UpdateData();

    GetMesh()->render();
}

void CSkyBox::SetType(SKYBOX_TYPE _type)
{
    m_Type = _type;

    if (SKYBOX_TYPE::IBLBaker == _type)
    {
        m_BrdfTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerBrdf.dds",
                                                         L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerBrdf.dds");
        m_EnvTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerEnvHDR.dds",
                                                        L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerEnvHDR.dds");
        m_DiffuseTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerDiffuseHDR.dds",
                                                            L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerDiffuseHDR.dds");
        m_SpecularTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerSpecularHDR.dds",
                                                             L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerSpecularHDR.dds");
    }
    else if (SKYBOX_TYPE::LearnOpenGL == _type)
    {
        m_BrdfTex = nullptr;
        m_EnvTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\LearnOpenGL\\LearnOpenGL_bgra.dds",
                                                        L"Developers\\Textures\\Cubemaps\\LearnOpenGL\\LearnOpenGL_bgra.dds");
        m_DiffuseTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\LearnOpenGL\\LearnOpenGL_diffuse.dds",
                                                            L"Developers\\Textures\\Cubemaps\\LearnOpenGL\\LearnOpenGL_diffuse.dds");
        m_SpecularTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\LearnOpenGL\\LearnOpenGL_specular.dds",
                                                             L"Developers\\Textures\\Cubemaps\\LearnOpenGL\\LearnOpenGL_specular.dds");
    }
    else if (SKYBOX_TYPE::moonless == _type)
    {
        m_BrdfTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\moonless\\moonlessBrdf.dds",
                                                         L"Developers\\Textures\\Cubemaps\\moonless\\moonlessBrdf.dds");
        m_EnvTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\moonless\\moonlessEnvHDR.dds",
                                                        L"Developers\\Textures\\Cubemaps\\moonless\\moonlessEnvHDR.dds");
        m_DiffuseTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\moonless\\moonlessDiffuseHDR.dds",
                                                            L"Developers\\Textures\\Cubemaps\\moonless\\moonlessDiffuseHDR.dds");
        m_SpecularTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\moonless\\moonlessSpecularHDR.dds",
                                                             L"Developers\\Textures\\Cubemaps\\moonless\\moonlessSpecularHDR.dds");
    }
    else if (SKYBOX_TYPE::PureSky == _type)
    {
        m_BrdfTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyBrdf.dds",
                                                         L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyBrdf.dds");
        m_EnvTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyEnvHDR.dds",
                                                        L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyEnvHDR.dds");
        m_DiffuseTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyDiffuseHDR.dds",
                                                            L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyDiffuseHDR.dds");
        m_SpecularTex = CAssetMgr::GetInst()->Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkySpecularHDR.dds",
                                                             L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkySpecularHDR.dds");
    }

    UpdateData();
}

void CSkyBox::SetShape(SKYBOX_SHAPE _Shape)
{
    m_Shape = _Shape;

    if (SKYBOX_SHAPE::SPHERE == _Shape)
    {
        SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    }

    else if (SKYBOX_SHAPE::BOX == _Shape)
    {
        SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    }

    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}

void CSkyBox::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);

    fwrite(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
    fwrite(&m_Shape, sizeof(SKYBOX_SHAPE), 1, _File);
}

void CSkyBox::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);

    fread(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
    fread(&m_Shape, sizeof(SKYBOX_SHAPE), 1, _File);

    SetType(m_Type);
    SetShape(m_Shape);
}
