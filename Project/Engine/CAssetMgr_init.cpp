#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CModleLoader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"

#include "CSetColorShader.h"
#include "CParticleUpdate.h"

void CAssetMgr::CreateDefaultMesh()
{
    // Crosshair
    if (nullptr == FindAsset<CMesh>(L"CrosshairMesh"))
    {
        auto mesh = MakeCrosshair();

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CrosshairMesh");
        AddAsset(L"CrosshairMesh", pMesh);
    }

    // Point
    if (nullptr == FindAsset<CMesh>(L"PointMesh"))
    {
        auto mesh = MakePoint();

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"PointMesh");
        AddAsset(L"PointMesh", pMesh);
    }

    // Circle
    if (nullptr == FindAsset<CMesh>(L"CircleMesh"))
    {
        auto mesh = MakeCircle(1.f, 40);

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CircleMesh");
        AddAsset(L"CircleMesh", pMesh);
    }

    // Rect
    if (nullptr == FindAsset<CMesh>(L"RectMesh"))
    {
        auto mesh = MakeRect();

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"RectMesh");
        AddAsset(L"RectMesh", pMesh);
    }

    // Debug Circle
    if (nullptr == FindAsset<CMesh>(L"CircleMesh_Debug"))
    {
        auto mesh = MakeDebugCircle(1.f, 40);

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CircleMesh_Debug");
        AddAsset(L"CircleMesh_Debug", pMesh);
    }

    // Debug Rect
    if (nullptr == FindAsset<CMesh>(L"RectMesh_Debug"))
    {
        auto mesh = MakeDebugRect();

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"RectMesh_Debug");
        AddAsset(L"RectMesh_Debug", pMesh);
    }

    // SquareGrid
    if (nullptr == FindAsset<CMesh>(L"SquareGridMesh"))
    {
        auto mesh = MakeSquareGrid(10, 10);

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SquareGridMesh");
        AddAsset(L"SquareGridMesh", pMesh);
    }

    // Box
    if (nullptr == FindAsset<CMesh>(L"BoxMesh"))
    {
        auto mesh = MakeBox();

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"BoxMesh");
        AddAsset(L"BoxMesh", pMesh);
    }

    // Cylinder
    if (nullptr == FindAsset<CMesh>(L"CylinderMesh"))
    {
        auto mesh = MakeCylinder(1, 1, 1, 100);
        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CylinderMesh");
        AddAsset(L"CylinderMesh", pMesh);
    }

    // Sphere
    if (nullptr == FindAsset<CMesh>(L"SphereMesh"))
    {
        auto mesh = MakeSphere(1, 10, 10);

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SphereMesh");
        AddAsset(L"SphereMesh", pMesh);
    }

    // Tetrahedron
    if (nullptr == FindAsset<CMesh>(L"TetrahedronMesh"))
    {
        auto mesh = MakeTetrahedron();
        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"TetrahedronMesh");
        AddAsset(L"TetrahedronMesh", pMesh);
    }

    // Icosahedron
    if (nullptr == FindAsset<CMesh>(L"IcosahedronMesh"))
    {
        auto mesh = MakeIcosahedron();
        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"IcosahedronMesh");
        AddAsset(L"IcosahedronMesh", pMesh);
    }

    // Sphere
    if (nullptr == FindAsset<CMesh>(L"SubdivideSphereMesh"))
    {
        auto mesh = MakeSphere(1, 25, 25);
        mesh = SubdivideToSphere(1.f, mesh);

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SubdivideSphereMesh");
        AddAsset(L"SubdivideSphereMesh", pMesh);
    }
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
    // =================================
    // Std2DShader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"Std2DShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->SetName(L"Std2DShader");
        AddAsset(L"Std2DShader", pShader);
    }

    // =================================
    // EffectShader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"EffectShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Effect");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->SetName(L"EffectShader");
        AddAsset(L"EffectShader", pShader);
    }

    // =============
    // TileMapShader
    // =============
    if (nullptr == FindAsset<CGraphicsShader>(L"TileMapShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\Tilemap.fx", "VS_TileMap");
        pShader->CreatePixelShader(L"shader\\Tilemap.fx", "PS_TileMap");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->SetName(L"TileMapShader");
        AddAsset(L"TileMapShader", pShader);
    }

    // =============
    // ParticleRender
    // =============
    if (nullptr == FindAsset<CGraphicsShader>(L"ParticleRenderShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\particle_render.fx", "VS_ParticleRender");
        pShader->CreateGeometryShader(L"shader\\particle_render.fx", "GS_ParticleRender");
        pShader->CreatePixelShader(L"shader\\particle_render.fx", "PS_ParticleRender");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        pShader->SetName(L"ParticleRenderShader");
        AddAsset(L"ParticleRenderShader", pShader);
    }

    // =================================
    // GrayFilter Shader
    // Mesh			: RectMesh
    // RS_TYPE		: CULL_BACK
    // DS_TYPE		: NO_TEST_NO_WRITE
    // BS_TYPE		: Default
    // Domain		: DOMAIN_POSTPROCESS
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"GrayFilterShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayFilter");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"GrayFilterShader");
        AddAsset(L"GrayFilterShader", pShader);
    }

    // =================================
    // Distortion Shader
    // RS_TYPE		: CULL_BACK
    // DS_TYPE		: NO_TEST_NO_WRITE
    // BS_TYPE		: Default
    // Domain		: DOMAIN_POSTPROCESS
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"DistortionShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"DistortionShader");
        AddAsset(L"DistortionShader", pShader);
    }

    // =================================
    // DebugShape Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"DebugShapeShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
        pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetBSType(BS_TYPE::DEFAULT);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

        pShader->SetName(L"DebugShapeShader");
        AddAsset(L"DebugShapeShader", pShader);
    }

    // =================================
    // Basic Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"BasicShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\BasicVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BasicPS.hlsl", "main");

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->SetName(L"BasicShader");
        AddAsset(L"BasicShader", pShader);
    }

    // =================================
    // BlinnPhong Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"BlinnPhongShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\BlinnPhongVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BlinnPhongPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"BlinnPhongShader");
        AddAsset(L"BlinnPhongShader", pShader);
    }

    // =================================
    // NormalLine Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"NormalLineShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\NormalLineVS.hlsl", "main");
        pShader->CreateGeometryShader(L"shader\\NormalLineGS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\NormalLinePS.hlsl", "main");

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->SetName(L"NormalLineShader");
        AddAsset(L"NormalLineShader", pShader);
    }

    // =================================
    // 2D OutLine Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"2D_OutLineShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\2D_OutLineVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\2D_OutLinePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"2D_OutLineShader");
        AddAsset(L"2D_OutLineShader", pShader);
    }

    // =================================
    // 3D OutLine Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"3D_OutLineShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\3D_OutLineVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\3D_OutLinePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT); // OutLine은 뒷면만 렌더링
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"3D_OutLineShader");
        AddAsset(L"3D_OutLineShader", pShader);
    }

    // =================================
    // SkyBox Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"SkyboxShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\SkyBoxVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\SkyboxPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT); // SkyBox는 뒷면만 렌더링
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"SkyboxShader");
        AddAsset(L"SkyboxShader", pShader);
    }

    // =================================
    // BillBoardPoint Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"BillBoardPointShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\BillBoardPointVS.hlsl", "main");
        pShader->CreateGeometryShader(L"shader\\BillBoardPointGS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BillBoardPointPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"BillBoardPointShader");
        AddAsset(L"BillBoardPointShader", pShader);
    }

    // =================================
    // IDMap Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"IDMapShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\IDMapVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\IDMapPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS_EQUAL);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"IDMapShader");
        AddAsset(L"IDMapShader", pShader);
    }

    // =================================
    // SkyBox IDMap Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"SkyBox_IDMapShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\IDMapVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\IDMapPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->SetName(L"SkyBox_IDMapShader");
        AddAsset(L"SkyBox_IDMapShader", pShader);
    }

    // =================================
    // Bloom Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"BloomShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BloomPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"BloomShader");
        AddAsset(L"BloomShader", pShader);
    }

    // =================================
    // BlurX Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"BlurXShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BlurXPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"BlurXShader");
        AddAsset(L"BlurXShader", pShader);
    }

    // =================================
    // BlurY Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"BlurYShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BlurYPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"BlurYShader");
        AddAsset(L"BlurYShader", pShader);
    }

    // =================================
    // Shock Wave Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"ShockWaveShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\ShockWaveVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ShockWavePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"ShockWaveShader");
        AddAsset(L"ShockWaveShader", pShader);
    }

    // =================================
    // Tone Mapping Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"ToneMappingShader"))
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ToneMappingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"ToneMappingShader");
        AddAsset(L"ToneMappingShader", pShader);
    }
}

void CAssetMgr::CreateDefaultComputeShader()
{
    // =================================
    // SetColor Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"SetColorShader"))
    {
        Ptr<CComputeShader> pShader = new CSetColorShader;
        pShader->SetName(L"SetColorShader");
        AddAsset(L"SetColorShader", pShader);
    }

    // =================================
    // SetColor Shader
    // =================================
    if (nullptr == FindAsset<CGraphicsShader>(L"ParticleUpdateShader"))
    {
        Ptr<CComputeShader> pShader = new CParticleUpdate;
        pShader->SetName(L"ParticleUpdateShader");
        AddAsset(L"ParticleUpdateShader", pShader);
    }
}
void CAssetMgr::CreateDefaultTexture()
{
    if (nullptr == FindAsset<CTexture>(L"missing_texture"))
    {
        Ptr<CTexture> ptex = Load<CTexture>(L"missing_texture", L"Textures\\missing_texture.png");
        assert(ptex.Get());
    }

    if (nullptr == FindAsset<CTexture>(L"DirectoryIconTex"))
        Load<CTexture>(L"DirectoryIconTex", L"Icons\\ContentBrowser\\DirectoryIcon.png");

    if (nullptr == FindAsset<CTexture>(L"FileIconTex"))
        Load<CTexture>(L"FileIconTex", L"Icons\\ContentBrowser\\FileIcon.png");

    if (nullptr == FindAsset<CTexture>(L"PauseButtonTex"))
        Load<CTexture>(L"PauseButtonTex", L"Icons\\PauseButton.png");
    if (nullptr == FindAsset<CTexture>(L"PlayButtonTex"))
        Load<CTexture>(L"PlayButtonTex", L"Icons\\PlayButton.png");
    if (nullptr == FindAsset<CTexture>(L"SimulateButtonTex"))
        Load<CTexture>(L"SimulateButtonTex", L"Icons\\SimulateButton.png");
    if (nullptr == FindAsset<CTexture>(L"StepButtonTex"))
        Load<CTexture>(L"StepButtonTex", L"Icons\\StepButton.png");
    if (nullptr == FindAsset<CTexture>(L"StopButtonTex"))
        Load<CTexture>(L"StopButtonTex", L"Icons\\StopButton.png");

    if (nullptr == FindAsset<CTexture>(L"DirectionalLightTex"))
        Load<CTexture>(L"DirectionalLightTex", L"Icons\\DirectionalLight.png");
    if (nullptr == FindAsset<CTexture>(L"PointLightTex"))
        Load<CTexture>(L"PointLightTex", L"Icons\\PointLight.png");
    if (nullptr == FindAsset<CTexture>(L"SpotLightTex"))
        Load<CTexture>(L"SpotLightTex", L"Icons\\SpotLight.png");

    // SkyBox
    if (nullptr == FindAsset<CTexture>(L"cubemapTex"))
        Load<CTexture>(L"cubemapTex", L"Developers\\Textures\\Cubemaps\\skybox\\cubemap_bgra.dds");
    if (nullptr == FindAsset<CTexture>(L"cubemap_diffuseTex"))
        Load<CTexture>(L"cubemap_diffuseTex", L"Developers\\Textures\\Cubemaps\\skybox\\cubemap_diffuse.dds");
    if (nullptr == FindAsset<CTexture>(L"cubemap_specularTex"))
        Load<CTexture>(L"cubemap_specularTex", L"Developers\\Textures\\Cubemaps\\skybox\\cubemap_specular.dds");

    if (nullptr == FindAsset<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyEnvHDR.dds"))
        Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyEnvHDR.dds",
                       L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyEnvHDR.dds");
    if (nullptr == FindAsset<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyDiffuseHDR.dds"))
        Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyDiffuseHDR.dds",
                       L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyDiffuseHDR.dds");
    if (nullptr == FindAsset<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkySpecularHDR.dds"))
        Load<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkySpecularHDR.dds",
                       L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkySpecularHDR.dds");

    // Noise Texture Load
    if (nullptr == FindAsset<CTexture>(L"Textures\\noise\\noise_01.jpg"))
        CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\noise\\noise_01.jpg", L"Textures\\noise\\noise_01.jpg");
    if (nullptr == FindAsset<CTexture>(L"Textures\\noise\\noise_02.jpg"))
        CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\noise\\noise_02.jpg", L"Textures\\noise\\noise_02.jpg");
    if (nullptr == FindAsset<CTexture>(L"Textures\\noise\\noise_03.jpg"))
        CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\noise\\noise_03.jpg", L"Textures\\noise\\noise_03.jpg");

    if (nullptr == FindAsset<CTexture>(L"BlueprintBackgroundTex"))
        Load<CTexture>(L"BlueprintBackgroundTex", L"Icons\\Blueprint\\BlueprintBackground.png");
    if (nullptr == FindAsset<CTexture>(L"ic_restore_white_24dpTex"))
        Load<CTexture>(L"ic_restore_white_24dpTex", L"Icons\\Blueprint\\ic_restore_white_24dp.png");
    if (nullptr == FindAsset<CTexture>(L"ic_save_white_24dpTex"))
        Load<CTexture>(L"ic_save_white_24dpTex", L"Icons\\Blueprint\\ic_save_white_24dp.png");

    if (nullptr == FindAsset<CTexture>(L"MapTileTex"))
        Load<CTexture>(L"MapTileTex", L"EnterTheGungeon\\Map\\MapTile.bmp");
    if (nullptr == FindAsset<CTexture>(L"MapTileSmallTex"))
        Load<CTexture>(L"MapTileSmallTex", L"EnterTheGungeon\\Map\\MapTileSmall.bmp");
}

void CAssetMgr::CreateDefaultMaterial()
{
    // Std2DMtrl
    if (nullptr == FindAsset<CMaterial>(L"Std2DMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
        pMtrl->SetName(L"Std2DMtrl");
        AddAsset<CMaterial>(L"Std2DMtrl", pMtrl);
    }

    // TileMapMtrl
    if (nullptr == FindAsset<CMaterial>(L"TileMapMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"TileMapShader"));
        pMtrl->SetName(L"TileMapMtrl");
        AddAsset<CMaterial>(L"TileMapMtrl", pMtrl);
    }

    // ParticleRenderMtrl
    if (nullptr == FindAsset<CMaterial>(L"ParticleRenderMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ParticleRenderShader"));
        pMtrl->SetName(L"ParticleRenderMtrl");
        AddAsset<CMaterial>(L"ParticleRenderMtrl", pMtrl);
    }

    // GrayFilterMtrl
    if (nullptr == FindAsset<CMaterial>(L"GrayFilterMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"GrayFilterShader"));
        pMtrl->SetName(L"GrayFilterMtrl");
        AddAsset<CMaterial>(L"GrayFilterMtrl", pMtrl);
    }

    // DistortionMtrl
    if (nullptr == FindAsset<CMaterial>(L"DistortionMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DistortionShader"));
        pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"Textures\\noise\\noise_01.jpg"));
        pMtrl->SetName(L"DistortionMtrl");
        AddAsset<CMaterial>(L"DistortionMtrl", pMtrl);
    }

    // DebugShapeMtrl
    if (nullptr == FindAsset<CMaterial>(L"DebugShapeMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DebugShapeShader"));
        pMtrl->SetName(L"DebugShapeMtrl");
        AddAsset<CMaterial>(L"DebugShapeMtrl", pMtrl);
    }

    // Basic
    if (nullptr == FindAsset<CMaterial>(L"BasicMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BasicShader"));
        pMtrl->SetMaterialCoefficient(Vec4(1.f, 1.f, 0.f, 1.f), Vec4(0.5f, 0.5f, 0.5f, 1.f),
                                      Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4());
        pMtrl->SetName(L"BasicMtrl");
        AddAsset<CMaterial>(L"BasicMtrl", pMtrl);
    }

    // BlinnPhong
    if (nullptr == FindAsset<CMaterial>(L"BlinnPhongMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlinnPhongShader"));
        pMtrl->SetMaterialCoefficient(Vec4(), Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4());
        pMtrl->SetTexParam(TEXCUBE_0, FindAsset<CTexture>(L"cubemap_diffuseTex"));
        pMtrl->SetTexParam(TEXCUBE_1, FindAsset<CTexture>(L"cubemap_specularTex"));
        pMtrl->SetName(L"BlinnPhongMtrl");
        AddAsset<CMaterial>(L"BlinnPhongMtrl", pMtrl);
    }

    // SkyBox CubeMap
    if (nullptr == FindAsset<CMaterial>(L"SkyboxMtrl"))
    {
        CMaterial* pMtrl = nullptr;
        pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SkyboxShader"));
        pMtrl->SetTexParam(TEXCUBE_0, FindAsset<CTexture>(L"cubemapTex"));
        pMtrl->SetName(L"SkyboxMtrl");
        AddAsset<CMaterial>(L"SkyboxMtrl", pMtrl);
    }

    // PureSky CubeMap
    if (nullptr == FindAsset<CMaterial>(L"PureSkyBoxMtrl"))
    {
        CMaterial* pMtrl = nullptr;
        pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SkyboxShader"));
        pMtrl->SetTexParam(TEXCUBE_0,
                           FindAsset<CTexture>(L"Developers\\Textures\\Cubemaps\\PureSky\\PureSkyEnvHDR.dds"));
        pMtrl->SetName(L"PureSkyBoxMtrl");
        AddAsset<CMaterial>(L"PureSkyBoxMtrl", pMtrl);
    }

    // Directional Light
    if (nullptr == FindAsset<CMaterial>(L"DirectionalLightMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"DirectionalLightTex"));
        pMtrl->SetName(L"DirectionalLightMtrl");
        AddAsset<CMaterial>(L"DirectionalLightMtrl", pMtrl);
    }

    // Point Light
    if (nullptr == FindAsset<CMaterial>(L"PointLightMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PointLightTex"));
        pMtrl->SetName(L"PointLightMtrl");
        AddAsset<CMaterial>(L"PointLightMtrl", pMtrl);
    }

    // Spot Light
    if (nullptr == FindAsset<CMaterial>(L"SpotLightMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"SpotLightTex"));
        pMtrl->SetName(L"SpotLightMtrl");
        AddAsset<CMaterial>(L"SpotLightMtrl", pMtrl);
    }

    // Bloom
    if (nullptr == FindAsset<CMaterial>(L"BloomMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BloomShader"));
        pMtrl->SetScalarParam(FLOAT_0, 1.f);  // Strength
        pMtrl->SetScalarParam(FLOAT_1, 0.5f); // Threshold
        pMtrl->SetName(L"BloomMtrl");
        AddAsset<CMaterial>(L"BloomMtrl", pMtrl);
    }

    // BlurX
    if (nullptr == FindAsset<CMaterial>(L"BlurXMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlurXShader"));
        pMtrl->SetName(L"BlurXMtrl");
        AddAsset<CMaterial>(L"BlurXMtrl", pMtrl);
    }

    // BlurY
    if (nullptr == FindAsset<CMaterial>(L"BlurYMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlurYShader"));
        pMtrl->SetName(L"BlurYMtrl");
        AddAsset<CMaterial>(L"BlurYMtrl", pMtrl);
    }

    // ShockWave
    if (nullptr == FindAsset<CMaterial>(L"ShockWaveMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ShockWaveShader"));
        pMtrl->SetName(L"ShockWaveMtrl");
        AddAsset<CMaterial>(L"ShockWaveMtrl", pMtrl);
    }

    // ToneMapping
    if (nullptr == FindAsset<CMaterial>(L"ToneMappingMtrl"))
    {
        CMaterial* pMtrl = new CMaterial;
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ToneMappingShader"));
        pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"FloatRenderTargetTexture"));
        pMtrl->SetScalarParam(FLOAT_0, 1.f); // Exposure
        pMtrl->SetScalarParam(FLOAT_1, 2.2f); // Gamma
        pMtrl->SetName(L"ToneMappingMtrl");
        AddAsset<CMaterial>(L"ToneMappingMtrl", pMtrl);
    }
}

tMeshData CAssetMgr::MakeCrosshair()
{
    vector<Vec3> positions;
    vector<Vec3> colors;
    vector<Vec2> texcoords;

    tMeshData meshData;
    positions.push_back(Vec3(0.f, 0.5f, 0.f));
    positions.push_back(Vec3(0.f, -0.5f, 0.f));
    positions.push_back(Vec3(-0.5f, 0.f, 0.f));
    positions.push_back(Vec3(0.5f, 0.f, 0.f));

    texcoords.push_back(Vec2(0.f, 0.f));
    texcoords.push_back(Vec2(0.f, 0.f));
    texcoords.push_back(Vec2(0.f, 0.f));
    texcoords.push_back(Vec2(0.f, 0.f));

    colors.push_back(Vec3(0.f, 1.f, 0.f));
    colors.push_back(Vec3(0.f, 1.f, 0.f));
    colors.push_back(Vec3(0.f, 1.f, 0.f));
    colors.push_back(Vec3(0.f, 1.f, 0.f));

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vUV = texcoords[i];
        v.vColor = colors[i];
        v.vColor.w = 1.f;

        meshData.vertices.push_back(v);
    }

    meshData.indices.push_back(0);
    meshData.indices.push_back(1);
    meshData.indices.push_back(2);
    meshData.indices.push_back(3);

    return meshData;
}

tMeshData CAssetMgr::MakePoint()
{
    tMeshData meshData;
    Vtx v;
    v.vPos = Vec3(0.f, 0.f, 0.f);
    v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    v.vUV = Vec2(0.5f, 0.5f);
    meshData.vertices.push_back(v);
    meshData.indices.push_back(0);

    return meshData;
}

tMeshData CAssetMgr::MakeCircle(const float radius, const int numSlices)
{
    tMeshData meshData;

    Vtx v;

    // 중심 점
    v.vPos = Vec3(0.f, 0.f, 0.f);
    v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    v.vUV = Vec2(0.5f, 0.5f);
    meshData.vertices.push_back(v);

    float fTheta = 0.f;

    for (int i = 0; i <= numSlices; ++i)
    {
        fTheta = (XM_2PI / numSlices) * i;

        v.vPos = Vec3(radius * cosf(fTheta), radius * sinf(fTheta), 0.f);
        v.vNormal = Vec3(0.f, 0.f, -1.f);
        v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
        v.vUV = Vec2(cosf(fTheta), sinf(fTheta));

        meshData.vertices.push_back(v);
    }

    for (int i = 0; i < numSlices; ++i)
    {
        meshData.indices.push_back(0);
        meshData.indices.push_back(i + 2);
        meshData.indices.push_back(i + 1);
    }

    return meshData;
}

tMeshData CAssetMgr::MakeRect(const float scale, const Vec2 texScale)
{
    vector<Vec3> positions;
    vector<Vec3> colors;
    vector<Vec3> normals;
    vector<Vec2> texcoords; // 텍스춰 좌표

    // 앞면
    positions.push_back(Vec3(-0.5f, 0.5f, 0.0f) * scale);
    positions.push_back(Vec3(0.5f, 0.5f, 0.0f) * scale);
    positions.push_back(Vec3(0.5f, -0.5f, 0.0f) * scale);
    positions.push_back(Vec3(-0.5f, -0.5f, 0.0f) * scale);
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));

    // Texture Coordinates (Direct3D 9)
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    tMeshData meshData;

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vNormal = normals[i];
        v.vUV = texcoords[i] * texScale;
        // v.tangentModel = Vec3(1.0f, 0.0f, 0.0f);
        v.vColor = colors[i];
        v.vColor.w = 1.f;

        meshData.vertices.push_back(v);
    }
    meshData.indices = {
        0, 1, 2, 0, 2, 3, // 앞면
    };

    return meshData;
}

tMeshData CAssetMgr::MakeDebugCircle(const float radius, const int numSlices)
{
    tMeshData meshData;

    Vtx v;

    // 중심 점
    v.vPos = Vec3(0.f, 0.f, 0.f);
    v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    v.vUV = Vec2(0.5f, 0.5f);
    meshData.vertices.push_back(v);

    float fTheta = 0.f;

    for (int i = 0; i <= numSlices; ++i)
    {
        fTheta = (XM_2PI / numSlices) * i;

        v.vPos = Vec3(radius * cosf(fTheta), radius * sinf(fTheta), 0.f);
        v.vNormal = Vec3(0.f, 0.f, -1.f);
        v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
        v.vUV = Vec2(cosf(fTheta), sinf(fTheta));

        meshData.vertices.push_back(v);
    }

    for (int i = 1; i < meshData.vertices.size(); ++i)
    {
        meshData.indices.push_back(i);
    }

    return meshData;
}

tMeshData CAssetMgr::MakeDebugRect(const float scale, const Vec2 texScale)
{
    vector<Vec3> positions;
    vector<Vec3> colors;
    vector<Vec3> normals;
    vector<Vec2> texcoords; // 텍스춰 좌표

    // 앞면
    positions.push_back(Vec3(-0.5f, 0.5f, 0.0f) * scale);
    positions.push_back(Vec3(0.5f, 0.5f, 0.0f) * scale);
    positions.push_back(Vec3(0.5f, -0.5f, 0.0f) * scale);
    positions.push_back(Vec3(-0.5f, -0.5f, 0.0f) * scale);
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));

    // Texture Coordinates (Direct3D 9)
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    tMeshData meshData;

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vNormal = normals[i];
        v.vUV = texcoords[i] * texScale;
        // v.tangentModel = Vec3(1.0f, 0.0f, 0.0f);
        v.vColor = colors[i];
        v.vColor.w = 1.f;

        meshData.vertices.push_back(v);
    }

    meshData.indices = {0, 1, 2, 3, 0};

    return meshData;
}

tMeshData CAssetMgr::MakeSquareGrid(const int numSlices, const int numStacks, const float scale, const Vec2 texScale)
{
    tMeshData meshData;

    float dx = 2.0f / numSlices;
    float dy = 2.0f / numStacks;

    float y = 1.0f;
    for (int j = 0; j < numStacks + 1; j++)
    {
        float x = -1.0f;
        for (int i = 0; i < numSlices + 1; i++)
        {
            Vtx v;
            v.vPos = Vec3(x, y, 0.0f) * scale;
            v.vNormal = Vec3(0.0f, 0.0f, -1.0f);
            v.vUV = Vec2(x + 1.0f, y + 1.0f) * 0.5f * texScale;
            // v.tangentModel = Vec3(1.0f, 0.0f, 0.0f);

            meshData.vertices.push_back(v);

            x += dx;
        }
        y -= dy;
    }

    for (int j = 0; j < numStacks; j++)
    {
        for (int i = 0; i < numSlices; i++)
        {
            meshData.indices.push_back((numSlices + 1) * j + i);
            meshData.indices.push_back((numSlices + 1) * j + i + 1);
            meshData.indices.push_back((numSlices + 1) * (j + 1) + i);
            meshData.indices.push_back((numSlices + 1) * (j + 1) + i);
            meshData.indices.push_back((numSlices + 1) * j + i + 1);
            meshData.indices.push_back((numSlices + 1) * (j + 1) + i + 1);
        }
    }

    return meshData;
}

tMeshData CAssetMgr::MakeBox(const float scale)
{
    vector<Vec3> positions;
    vector<Vec3> colors;
    vector<Vec3> normals;
    vector<Vec2> texcoords; // 텍스춰 좌표

    // 윗면
    positions.push_back(Vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vec3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(Vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 아랫면
    positions.push_back(Vec3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vec3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vec3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(Vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 앞면
    positions.push_back(Vec3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vec3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 뒷면
    positions.push_back(Vec3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vec3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vec3(0.0f, 1.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 왼쪽
    positions.push_back(Vec3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vec3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vec3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vec3(1.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 오른쪽
    positions.push_back(Vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vec3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vec3(1.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    tMeshData meshData;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vNormal = normals[i];
        v.vUV = texcoords[i];
        v.vColor = colors[i];
        v.vColor.w = 1.f;
        meshData.vertices.push_back(v);
    }

    meshData.indices = {
        0,  1,  2,  0,  2,  3,  // 윗면
        4,  5,  6,  4,  6,  7,  // 아랫면
        8,  9,  10, 8,  10, 11, // 앞면
        12, 13, 14, 12, 14, 15, // 뒷면
        16, 17, 18, 16, 18, 19, // 왼쪽
        20, 21, 22, 20, 22, 23  // 오른쪽
    };

    return meshData;
}

tMeshData CAssetMgr::MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices)
{
    // Texture 좌표계때문에 (numSlices + 1) x 2 개의 버텍스 사용

    const float dTheta = -XM_2PI / float(numSlices);

    tMeshData meshData;

    vector<Vtx>& vertices = meshData.vertices;

    // 옆면의 바닥 버텍스들 (인덱스 0 이상 numSlices 미만)
    for (int i = 0; i <= numSlices; i++)
    {
        Vtx v;
        v.vPos = Vec3::Transform(Vec3(bottomRadius, -0.5f * height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));

        // std::cout << v.vPos.x << " " << v.vPos.z << std::endl;

        v.vNormal = v.vPos - Vec3(0.0f, -0.5f * height, 0.0f);
        v.vNormal.Normalize();
        v.vUV = Vec2(float(i) / numSlices, 1.0f);

        vertices.push_back(v);
    }

    // 옆면의 맨 위 버텍스들 (인덱스 numSlices 이상 2 * numSlices 미만)
    for (int i = 0; i <= numSlices; i++)
    {
        Vtx v;
        v.vPos = Vec3::Transform(Vec3(topRadius, 0.5f * height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));
        v.vNormal = v.vPos - Vec3(0.0f, 0.5f * height, 0.0f);
        v.vNormal.Normalize();
        v.vUV = Vec2(float(i) / numSlices, 0.0f);

        vertices.push_back(v);
    }

    vector<UINT>& indices = meshData.indices;

    for (int i = 0; i < numSlices; i++)
    {
        indices.push_back(i);
        indices.push_back(i + numSlices + 1);
        indices.push_back(i + 1 + numSlices + 1);

        indices.push_back(i);
        indices.push_back(i + 1 + numSlices + 1);
        indices.push_back(i + 1);
    }

    return meshData;
}

tMeshData CAssetMgr::MakeSphere(const float radius, const int numSlices, const int numStacks, const Vec2 texScale)
{

    // 참고: OpenGL Sphere
    // http://www.songho.ca/opengl/gl_sphere.html
    // Texture 좌표계때문에 (numSlices + 1) 개의 버텍스 사용 (마지막에 닫아주는
    // 버텍스가 중복) Stack은 y 위쪽 방향으로 쌓아가는 방식

    const float dTheta = -XM_2PI / float(numSlices);
    const float dPhi = -XM_PI / float(numStacks);

    tMeshData meshData;

    vector<Vtx>& vertices = meshData.vertices;

    for (int j = 0; j <= numStacks; j++)
    {

        // 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
        Vec3 stackStartPoint = Vec3::Transform(Vec3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));

        for (int i = 0; i <= numSlices; i++)
        {
            Vtx v;

            // 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
            v.vPos = Vec3::Transform(stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

            v.vNormal = v.vPos; // 원점이 구의 중심
            v.vNormal.Normalize();
            v.vUV = Vec2(float(i) / numSlices, 1.0f - float(j) / numStacks) * texScale;

            // texcoord가 위로 갈수록 증가
            Vec3 biTangent = Vec3(0.0f, 1.0f, 0.0f);

            Vec3 normalOrth = v.vNormal - biTangent.Dot(v.vNormal) * v.vNormal;
            normalOrth.Normalize();

            // v.tangentModel = biTangent.Cross(normalOrth);
            // v.tangentModel.Normalize();

            /*    Vec3::Transform(Vec3(0.0f, 0.0f, 1.0f),
                                   Matrix::CreateRotationY(dTheta *
               float(i)));*/
            // v.biTangentModel = Vec3(0.0f, 1.0f, 0.0f);

            vertices.push_back(v);
        }
    }

    // cout << vertices.size() << endl;

    vector<UINT>& indices = meshData.indices;

    for (int j = 0; j < numStacks; j++)
    {
        const int offset = (numSlices + 1) * j;

        for (int i = 0; i < numSlices; i++)
        {
            indices.push_back(offset + i);
            indices.push_back(offset + i + numSlices + 1);
            indices.push_back(offset + i + 1 + numSlices + 1);

            indices.push_back(offset + i);
            indices.push_back(offset + i + 1 + numSlices + 1);
            indices.push_back(offset + i + 1);
        }
    }

    return meshData;
}

tMeshData CAssetMgr::MakeTetrahedron()
{
    // Regular Tetrahedron
    // https://mathworld.wolfram.com/RegularTetrahedron.html

    const float a = 1.0f;
    const float x = sqrt(3.0f) / 3.0f * a;
    const float d = sqrt(3.0f) / 6.0f * a; // = x / 2
    const float h = sqrt(6.0f) / 3.0f * a;

    vector<Vec3> points = {{0.0f, x, 0.0f}, {-0.5f * a, -d, 0.0f}, {+0.5f * a, -d, 0.0f}, {0.0f, 0.0f, h}};

    Vec3 center = Vec3(0.0f);

    for (int i = 0; i < 4; i++)
    {
        center += points[i];
    }
    center /= 4.0f;

    for (int i = 0; i < 4; i++)
    {
        points[i] -= center;
    }

    tMeshData meshData;

    for (int i = 0; i < points.size(); i++)
    {
        Vtx v;
        v.vPos = points[i];
        v.vNormal = v.vPos; // 중심이 원점
        v.vNormal.Normalize();

        meshData.vertices.push_back(v);
    }

    meshData.indices = {0, 1, 2, 3, 2, 1, 0, 3, 1, 0, 2, 3};

    return meshData;
}

tMeshData CAssetMgr::MakeIcosahedron()
{
    // Luna DX12 교재 참고
    // 등20면체
    // https://mathworld.wolfram.com/Isohedron.html

    const float X = 0.525731f;
    const float Z = 0.850651f;

    tMeshData newMesh;

    vector<Vec3> pos = {Vec3(-X, 0.0f, Z), Vec3(X, 0.0f, Z),  Vec3(-X, 0.0f, -Z), Vec3(X, 0.0f, -Z),
                        Vec3(0.0f, Z, X),  Vec3(0.0f, Z, -X), Vec3(0.0f, -Z, X),  Vec3(0.0f, -Z, -X),
                        Vec3(Z, X, 0.0f),  Vec3(-Z, X, 0.0f), Vec3(Z, -X, 0.0f),  Vec3(-Z, -X, 0.0f)};

    for (size_t i = 0; i < pos.size(); i++)
    {
        Vtx v;
        v.vPos = pos[i];
        v.vNormal = v.vPos;
        v.vNormal.Normalize();

        newMesh.vertices.push_back(v);
    }

    newMesh.indices = {1, 4,  0, 4, 9, 0,  4, 5, 9,  8, 5, 4,  1,  8,  4, 1, 10, 8,  10, 3,
                       8, 8,  3, 5, 3, 2,  5, 3, 7,  2, 3, 10, 7,  10, 6, 7, 6,  11, 7,  6,
                       0, 11, 6, 1, 0, 10, 1, 6, 11, 0, 9, 2,  11, 9,  5, 2, 9,  11, 2,  7};

    return newMesh;
}

tMeshData CAssetMgr::SubdivideToSphere(const float radius, tMeshData meshData)
{
    // 원점이 중심이라고 가정
    for (auto& v : meshData.vertices)
    {
        v.vPos = v.vNormal * radius;
    }

    // 구의 표면으로 옮기고 노멀과 texture 좌표 계산
    auto ProjectVertex = [&](Vtx& v) {
        v.vNormal = v.vPos;
        v.vNormal.Normalize();
        v.vPos = v.vNormal * radius;

        // 주의: 텍스춰가 이음매에서 깨집니다.
        // atan vs atan2
        // https://stackoverflow.com/questions/283406/what-is-the-difference-between-atan-and-atan2-in-c
        // const float theta = atan2f(v.vPos.z, v.vPos.x);
        // const float phi = acosf(v.vPos.y / radius);
        // v.vUV.x = theta / XM_2PI;
        // v.vUV.y = phi / XM_PI;
    };

    auto UpdateFaceNormal = [](Vtx& v0, Vtx& v1, Vtx& v2) {
        auto faceNormal = (v1.vPos - v0.vPos).Cross(v2.vPos - v0.vPos);
        faceNormal.Normalize();
        v0.vNormal = faceNormal;
        v1.vNormal = faceNormal;
        v2.vNormal = faceNormal;
    };

    // 버텍스가 중복되는 구조로 구현
    tMeshData newMesh;
    UINT count = 0;
    for (size_t i = 0; i < meshData.indices.size(); i += 3)
    {
        size_t i0 = meshData.indices[i];
        size_t i1 = meshData.indices[i + 1];
        size_t i2 = meshData.indices[i + 2];

        Vtx v0 = meshData.vertices[i0];
        Vtx v1 = meshData.vertices[i1];
        Vtx v2 = meshData.vertices[i2];

        Vtx v3;
        v3.vPos = (v0.vPos + v2.vPos) * 0.5f;
        v3.vUV = (v0.vUV + v2.vUV) * 0.5f;
        ProjectVertex(v3);

        Vtx v4;
        v4.vPos = (v0.vPos + v1.vPos) * 0.5f;
        v4.vUV = (v0.vUV + v1.vUV) * 0.5f;
        ProjectVertex(v4);

        Vtx v5;
        v5.vPos = (v1.vPos + v2.vPos) * 0.5f;
        v5.vUV = (v1.vUV + v2.vUV) * 0.5f;
        ProjectVertex(v5);

        // UpdateFaceNormal(v4, v1, v5);
        // UpdateFaceNormal(v0, v4, v3);
        // UpdateFaceNormal(v3, v4, v5);
        // UpdateFaceNormal(v3, v5, v2);

        newMesh.vertices.push_back(v4);
        newMesh.vertices.push_back(v1);
        newMesh.vertices.push_back(v5);

        newMesh.vertices.push_back(v0);
        newMesh.vertices.push_back(v4);
        newMesh.vertices.push_back(v3);

        newMesh.vertices.push_back(v3);
        newMesh.vertices.push_back(v4);
        newMesh.vertices.push_back(v5);

        newMesh.vertices.push_back(v3);
        newMesh.vertices.push_back(v5);
        newMesh.vertices.push_back(v2);

        for (UINT j = 0; j < 12; j++)
        {
            newMesh.indices.push_back(j + count);
        }
        count += 12;
    }

    return newMesh;
}
