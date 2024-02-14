#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"

#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CMeshRender.h"
#include "CCamera.h"

CLight3D::CLight3D()
    : CComponent(COMPONENT_TYPE::LIGHT3D)
    , m_Info{}
{
    m_Info.vRadiance = Vec4(1.f, 1.f, 1.f, 1.f);

    m_Info.fRadius = 0.f;
    m_Info.fAngle = 0.f;

    m_Info.LightType = (int)LIGHT_TYPE::POINT;

    m_Info.fallOffStart = 0.f;
    m_Info.fallOffEnd = 1000.f;
    m_Info.spotPower = 100.f;

    m_Info.CastShadow = 1;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    wstring name = L"LightDepthTex";
    name += std::to_wstring(GetID());
    m_DepthMapTex =
        CAssetMgr::GetInst()->CreateTexture(name, 1280, 1280, DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
                                            D3D11_USAGE_DEFAULT, &dsvDesc, nullptr, &srvDesc);
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
    // ±¤¿ø µî·Ï
    CRenderMgr::GetInst()->RegisterLight3D(this);

    m_Info.vWorldPos = Transform()->GetWorldPos();
    m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    // ±×¸²ÀÚ¸Ê
    Matrix ViewRow =
        XMMatrixLookAtLH(m_Info.vWorldPos, m_Info.vWorldPos + Transform()->GetWorldDir(DIR_TYPE::FRONT), Transform()->GetWorldDir(DIR_TYPE::UP));
    CCamera* mainCam = CRenderMgr::GetInst()->GetCamera(0);
    Matrix ProjRow = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), 1.0f, mainCam->GetNear(), mainCam->GetFar());

    m_Info.viewMat = ViewRow;
    m_Info.projMat = ProjRow;
    m_Info.invProj = m_Info.projMat.Invert();

    GamePlayStatic::DrawDebugSphere(m_Info.vWorldPos, m_Info.fRadius, Vec3(1.f, 1.f, 1.f), false);

    // Mesh ¼³Á¤
    if (nullptr == MeshRender())
        return;

    MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));

    if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirectionalLightMtrl"));
    else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl"));
    else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl"));
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Info, sizeof(tLightInfo), 1, _File);
}
