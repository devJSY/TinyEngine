#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"

#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CMeshRender.h"
#include "CCamera.h"
#include "CGameObjectEx.h"

CLight3D::CLight3D()
    : CComponent(COMPONENT_TYPE::LIGHT3D)
    , m_Info{}
    , m_pLightCam(nullptr)
    , m_VolumeMesh(nullptr)
    , m_LightMtrl(nullptr)
{
    m_Info.vRadiance = Vec4(1.f, 1.f, 1.f, 1.f);

    m_Info.fRadius = 0.f;
    m_Info.fAngle = 0.f;

    m_Info.LightType = (int)LIGHT_TYPE::POINT;

    m_Info.fallOffStart = 0.f;
    m_Info.fallOffEnd = 10.f;
    m_Info.spotPower = 100.f;

    m_Info.ShadowType = 1; // Dynamic Shadow
    m_Info.ShadowIndex = -1;

    CreateDepthMapTex();

    m_pLightCam = new CGameObjectEx;
    m_pLightCam->AddComponent(new CTransform);
    m_pLightCam->AddComponent(new CCamera);

    // 쉐이더와 동일하게 설정
    m_pLightCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
    m_pLightCam->Camera()->LayerMaskAll();
    m_pLightCam->Camera()->SetFOV(XMConvertToRadians(120.f));
    m_pLightCam->Camera()->SetNear(1.f);
    m_pLightCam->Camera()->SetFar(10000.f);
    m_pLightCam->Camera()->Resize(Vec2(m_DepthMapTex->GetWidth(), m_DepthMapTex->GetHeight()));

    SetLightType((LIGHT_TYPE)m_Info.LightType);
}

CLight3D::CLight3D(const CLight3D& origin)
    : CComponent(origin)
    , m_Info(origin.m_Info)
    , m_DepthMapTex(nullptr)
    , m_pLightCam(nullptr)
    , m_VolumeMesh(origin.m_VolumeMesh)
    , m_LightMtrl(origin.m_LightMtrl)
{
    CreateDepthMapTex();
    m_pLightCam = origin.m_pLightCam->Clone();
    SetLightType((LIGHT_TYPE)m_Info.LightType);
}

CLight3D::~CLight3D()
{
    if (nullptr != m_pLightCam)
    {
        delete m_pLightCam;
        m_pLightCam = nullptr;
    }
}

void CLight3D::finaltick()
{
    // 광원 등록
    CRenderMgr::GetInst()->RegisterLight3D(this);

    m_Info.vWorldPos = Transform()->GetWorldPos();
    m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    // 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트
    m_pLightCam->Transform()->SetRelativePos(Transform()->GetRelativePos());
    m_pLightCam->Transform()->SetRelativeRotation(Transform()->GetRelativeRotation());
    m_pLightCam->finaltick();

    // 쉐이더에서 사용할 광원의 행렬 설정
    m_Info.viewMat = m_pLightCam->Camera()->GetViewMat();
    m_Info.projMat = m_pLightCam->Camera()->GetProjMat();
    m_Info.invProj = m_Info.projMat.Invert();

    if (LIGHT_TYPE::DIRECTIONAL != (LIGHT_TYPE)m_Info.LightType)
    {
        Transform()->SetRelativeScale(Vec3(m_Info.fallOffEnd, m_Info.fallOffEnd, m_Info.fallOffEnd));
    }

    // 광원 범위 디버그 렌더
    if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
    {
        GamePlayStatic::DrawDebugSphere(m_Info.vWorldPos, m_Info.fallOffEnd, Vec3(1.f, 1.f, 1.f), true);
    }

    else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
    {
        GamePlayStatic::DrawDebugCone(Transform()->GetWorldMat(), Vec3(1.f, 1.f, 1.f), true);
    }
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
    m_Info.LightType = (int)_type;

    if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
    {
        m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
        m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredDirLightingMtrl");
        // m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLight_deferredMtrl"); // Phong
    }

    else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
    {
        m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
        m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredPointLightingMtrl");
        // m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLight_deferredMtrl"); // Phong
    }

    else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
    {
        m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
        m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredSpotLightingMtrl");
        // m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLight_deferredMtrl"); // Phong
    }

    // Mesh Render 설정
    if (nullptr == GetOwner() || nullptr == MeshRender())
        return;

    MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));

    if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
    {
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirectionalLightMtrl"));
    }
    else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
    {
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl"));
    }
    else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
    {
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl"));
    }
}

void CLight3D::render_Deferred(int _LightIdx)
{
    if (nullptr == m_VolumeMesh || nullptr == m_LightMtrl)
    {
        return;
    }

    Transform()->UpdateData();

    m_LightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, _LightIdx);

    if (LIGHT_TYPE::DIRECTIONAL != (LIGHT_TYPE)m_Info.LightType)
    {
        Matrix matWorldInv = Transform()->GetWorldMat().Invert();
        m_LightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, matWorldInv);
    }

    m_LightMtrl->UpdateData();

    m_VolumeMesh->render();
}

void CLight3D::render_LightDepth()
{
    m_pLightCam->Camera()->SortObject();
    m_pLightCam->Camera()->render_DepthOnly(m_DepthMapTex);
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
        CAssetMgr::GetInst()->CreateTexture(name, 2048, 2048, DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
                                            D3D11_USAGE_DEFAULT, &dsvDesc, nullptr, &srvDesc);
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Info, sizeof(tLightInfo), 1, _File);
    SetLightType((LIGHT_TYPE)m_Info.LightType);
}
