#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"

#include "CDevice.h"

CSkyBox::CSkyBox()
    : CRenderComponent(COMPONENT_TYPE::SKYBOX)
    , m_Type(SKYBOX_TYPE::SPHERE)
{
    SetType(m_Type);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::begin()
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
    if (nullptr == GetMesh() || nullptr == GetMaterial())
        return;

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

void CSkyBox::SetType(SKYBOX_TYPE _Type)
{
    m_Type = _Type;

    if (SKYBOX_TYPE::SPHERE == _Type)
    {
        SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    }

    else if (SKYBOX_TYPE::BOX == m_Type)
    {
        SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    }

    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}

void CSkyBox::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);

    fwrite(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);

    SaveAssetRef(m_BrdfTex.Get(), _File);
    SaveAssetRef(m_EnvTex.Get(), _File);
    SaveAssetRef(m_DiffuseTex.Get(), _File);
    SaveAssetRef(m_SpecularTex.Get(), _File);
}

void CSkyBox::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);

    fread(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
    SetType(m_Type);

    LoadAssetRef<CTexture>(m_BrdfTex, _File);
    LoadAssetRef<CTexture>(m_EnvTex, _File);
    LoadAssetRef<CTexture>(m_DiffuseTex, _File);
    LoadAssetRef<CTexture>(m_SpecularTex, _File);
}
