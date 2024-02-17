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
    , m_ShadowIdx(-1)
{
    m_Info.vRadiance = Vec4(1.f, 1.f, 1.f, 1.f);

    m_Info.fRadius = 0.f;
    m_Info.fAngle = 0.f;

    m_Info.LightType = (int)LIGHT_TYPE::POINT;

    m_Info.fallOffStart = 0.f;
    m_Info.fallOffEnd = 1000.f;
    m_Info.spotPower = 100.f;

    m_Info.ShadowType = 1; // Dynamic Shadow

    CreateDepthMapTex();
}

CLight3D::CLight3D(const CLight3D& origin)
    : CComponent(origin)
    , m_Info(origin.m_Info)
    , m_DepthMapTex(nullptr)
    , m_ShadowIdx(-1)
{
    CreateDepthMapTex();
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
    // 광원 등록
    CRenderMgr::GetInst()->RegisterLight3D(this);

    m_Info.vWorldPos = Transform()->GetWorldPos();
    m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    // 그림자맵
    if (1 == m_Info.ShadowType)
    {
        Matrix ViewRow = XMMatrixLookAtLH(m_Info.vWorldPos, m_Info.vWorldPos + m_Info.vWorldDir, Transform()->GetWorldDir(DIR_TYPE::UP));
        Matrix ProjRow = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.f), 1.f, 1.f, 10000.f); // 쉐이더에서도 동일하게 설정

        m_Info.viewMat = ViewRow;
        m_Info.projMat = ProjRow;
        m_Info.invProj = m_Info.projMat.Invert();

        // LIGHT_FRUSTUM_WIDTH 확인
        // Vector4 eye(0.0f, 0.0f, 0.0f, 1.0f);
        // Vector4 xLeft(-1.0f, -1.0f, 0.0f, 1.0f);
        // Vector4 xRight(1.0f, 1.0f, 0.0f, 1.0f);
        // eye = Vector4::Transform(eye, lightProjRow);
        // xLeft = Vector4::Transform(xLeft, lightProjRow.Invert());
        // xRight = Vector4::Transform(xRight, lightProjRow.Invert());
        // xLeft /= xLeft.w;
        // xRight /= xRight.w;
        // cout << "LIGHT_FRUSTUM_WIDTH = " << xRight.x - xLeft.x << endl;
    }
    else
    {
        m_ShadowIdx = -1;
    }

    GamePlayStatic::DrawDebugSphere(m_Info.vWorldPos, m_Info.fRadius, Vec3(1.f, 1.f, 1.f), true);
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
    m_Info.LightType = (int)_type;

    if (nullptr == GetOwner() || nullptr == MeshRender())
        return;

    MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));

    if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirectionalLightMtrl"));
    else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl"));
    else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl"));
}

void CLight3D::CreateDepthMapTex()
{
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

void CLight3D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Info, sizeof(tLightInfo), 1, _File);

    if (nullptr == m_DepthMapTex)
        CreateDepthMapTex();
}
