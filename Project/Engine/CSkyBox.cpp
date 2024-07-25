#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"

#include "CDevice.h"
#include "CAssetMgr.h"

CSkyBox::CSkyBox()
    : CRenderComponent(COMPONENT_TYPE::SKYBOX)
    , m_Shape(SKYBOX_SHAPE::BOX)
{
    SetShape(m_Shape);

    SetFrustumCheck(false);
    SetCastShadow(false);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::finaltick()
{
    CRenderComponent::finaltick();
}

void CSkyBox::UpdateData()
{
    nullptr != m_BrdfTex ? m_BrdfTex->UpdateData(17) : CTexture::Clear(17);
    nullptr != m_DiffuseTex ? m_DiffuseTex->UpdateData(18) : CTexture::Clear(18);
    nullptr != m_SpecularTex ? m_SpecularTex->UpdateData(19) : CTexture::Clear(19);
}

void CSkyBox::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial(0))
        return;

    UpdateData();

    Transform()->UpdateData();

    GetMaterial(0)->SetScalarParam(INT_0, (int)m_Shape);

    if (SKYBOX_SHAPE::SPHERE == m_Shape)
    {
        GetMaterial(0)->SetTexParam(TEX_0, m_EnvTex);
    }
    else
    {
        GetMaterial(0)->SetTexParam(TEXCUBE_0, m_EnvTex);
    }

    GetMaterial(0)->UpdateData();

    GetMesh()->render(0);
}

void CSkyBox::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == GetMesh() || nullptr == _mtrl)
        return;

    Transform()->UpdateData();

    _mtrl->SetScalarParam(INT_0, (int)m_Shape);

    if (SKYBOX_SHAPE::SPHERE == m_Shape)
    {
        _mtrl->SetTexParam(TEX_0, m_EnvTex);
    }
    else
    {
        _mtrl->SetTexParam(TEXCUBE_0, m_EnvTex);
    }

    _mtrl->UpdateData();

    GetMesh()->render(0);
}

void CSkyBox::render(UINT _Subset)
{
    render();
}

void CSkyBox::ClearData()
{
    CTexture::Clear(17);
    CTexture::Clear(18);
    CTexture::Clear(19);
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

    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"), 0);
    // SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl_V2"), 0);
}

UINT CSkyBox::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CRenderComponent::SaveToLevelFile(_File);

    fwrite(&m_Shape, sizeof(SKYBOX_SHAPE), 1, _File);
    MemoryByte += SaveAssetRef(m_EnvTex, _File);
    MemoryByte += SaveAssetRef(m_BrdfTex, _File);
    MemoryByte += SaveAssetRef(m_DiffuseTex, _File);
    MemoryByte += SaveAssetRef(m_SpecularTex, _File);

    MemoryByte += sizeof(SKYBOX_SHAPE);

    return MemoryByte;
}

UINT CSkyBox::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CRenderComponent::LoadFromLevelFile(_File);

    fread(&m_Shape, sizeof(SKYBOX_SHAPE), 1, _File);
    MemoryByte += LoadAssetRef(m_EnvTex, _File);
    MemoryByte += LoadAssetRef(m_BrdfTex, _File);
    MemoryByte += LoadAssetRef(m_DiffuseTex, _File);
    MemoryByte += LoadAssetRef(m_SpecularTex, _File);

    SetShape(m_Shape);

    MemoryByte += sizeof(SKYBOX_SHAPE);

    return MemoryByte;
}
