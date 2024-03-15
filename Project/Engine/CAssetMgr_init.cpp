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
    {
        auto mesh = MakeCrosshair();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CrosshairMesh");
        AddAsset(L"CrosshairMesh", pMesh);
    }

    // Point
    {
        auto mesh = MakePoint();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"PointMesh");
        AddAsset(L"PointMesh", pMesh);
    }

    // Circle
    {
        auto mesh = MakeCircle(1.f, 40);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CircleMesh");
        AddAsset(L"CircleMesh", pMesh);
    }

    // Rect
    {
        auto mesh = MakeRect();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"RectMesh");
        AddAsset(L"RectMesh", pMesh);
    }

    // Debug Circle
    {
        auto mesh = MakeDebugCircle(1.f, 40);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CircleMesh_Debug");
        AddAsset(L"CircleMesh_Debug", pMesh);
    }

    // Debug Rect
    {
        auto mesh = MakeDebugRect();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"RectMesh_Debug");
        AddAsset(L"RectMesh_Debug", pMesh);
    }

    // SquareGrid
    {
        auto mesh = MakeSquareGrid(100, 100);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SquareGridMesh");
        AddAsset(L"SquareGridMesh", pMesh);
    }

    // Box
    {
        auto mesh = MakeBox();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"BoxMesh");
        AddAsset(L"BoxMesh", pMesh);
    }

    // Cylinder
    {
        auto mesh = MakeCylinder(1, 1, 1, 100);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CylinderMesh");
        AddAsset(L"CylinderMesh", pMesh);
    }

    // Sphere
    {
        auto mesh = MakeSphere(1, 50, 50);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SphereMesh");
        AddAsset(L"SphereMesh", pMesh);
    }

    // Tetrahedron
    {
        auto mesh = MakeTetrahedron();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"TetrahedronMesh");
        AddAsset(L"TetrahedronMesh", pMesh);
    }

    // Icosahedron
    {
        auto mesh = MakeIcosahedron();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"IcosahedronMesh");
        AddAsset(L"IcosahedronMesh", pMesh);
    }

    // Sphere
    {
        auto mesh = MakeSphere(1, 25, 25);
        mesh = SubdivideToSphere(1.f, mesh);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SubdivideSphereMesh");
        AddAsset(L"SubdivideSphereMesh", pMesh);
    }

    // Wire Box
    {
        auto mesh = MakeWireBox(Vec3(), Vec3(1.f, 1.f, 1.f));
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireBox");
        AddAsset(L"WireBox", pMesh);
    }

    // Wire Box
    {
        auto mesh = MakeWireSphere(Vec3(), 1.f);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireSphere");
        AddAsset(L"WireSphere", pMesh);
    }
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
    // =================================
    // Std2DShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"Std2DShader");
        AddAsset(L"Std2DShader", pShader);
    }

    // =================================
    // Std2DLightShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Light");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"Std2DLightShader");
        AddAsset(L"Std2DLightShader", pShader);
    }

    // =================================
    // TileMapShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\Tilemap.fx", "VS_TileMap");
        pShader->CreatePixelShader(L"shader\\Tilemap.fx", "PS_TileMap");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(INT_0, "COUNT X");
        pShader->AddScalarParam(INT_1, "COUNT Y");
        pShader->AddScalarParam(VEC2_0, "Slice UV");
        pShader->AddTexParam(TEX_0, "Atlas Texture");

        pShader->SetName(L"TileMapShader");
        AddAsset(L"TileMapShader", pShader);
    }

    // =================================
    // ParticleRender
    // =================================
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

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"ParticleRenderShader");
        AddAsset(L"ParticleRenderShader", pShader);
    }

    // =================================
    // GrayFilter Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Postprocess");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"GrayFilterShader");
        AddAsset(L"GrayFilterShader", pShader);
    }

    // =================================
    // BlendFilter Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Postprocess");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_BlendFilter");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Blend Color");

        pShader->SetName(L"BlendFilterShader");
        AddAsset(L"BlendFilterShader", pShader);
    }

    // =================================
    // Distortion Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Postprocess_World");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddTexParam(TEX_0, "Noise Texture");

        pShader->SetName(L"DistortionShader");
        AddAsset(L"DistortionShader", pShader);
    }

    // =================================
    // DebugShape Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
        pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

        pShader->AddScalarParam(VEC4_0, "Color");

        pShader->SetName(L"DebugShapeShader");
        AddAsset(L"DebugShapeShader", pShader);
    }

    // =================================
    // Unreal PBR Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UnrealPBRPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddScalarParam(INT_0, "Invert NormalMapY");
        pShader->AddScalarParam(FLOAT_0, "HeightMapping Scale");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");

        pShader->AddTexParam(TEX_0, "Ambient Texture");
        pShader->AddTexParam(TEX_1, "Ambient Occlusion Texture");
        pShader->AddTexParam(TEX_2, "Normal Texture");
        pShader->AddTexParam(TEX_3, "Height Texture");
        pShader->AddTexParam(TEX_4, "MetallicRoughness Texture");
        pShader->AddTexParam(TEX_5, "Emissive Texture");

        pShader->SetName(L"UnrealPBRShader");
        AddAsset(L"UnrealPBRShader", pShader);
    }

    // =================================
    // BlinnPhong Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\BlinnPhongVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BlinnPhongPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_0, "Rim Power");

        pShader->SetName(L"BlinnPhongShader");
        AddAsset(L"BlinnPhongShader", pShader);
    }

    // =================================
    // NormalLine Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\NormalLineVS.hlsl", "main");
        pShader->CreateGeometryShader(L"shader\\NormalLineGS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\NormalLinePS.hlsl", "main");

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(INT_0, "Enable Normal Line");
        pShader->AddScalarParam(FLOAT_0, "Normal Line Scale");

        pShader->SetName(L"NormalLineShader");
        AddAsset(L"NormalLineShader", pShader);
    }

    // =================================
    // 2D OutLine Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\2D_OutLineVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\2D_OutLinePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddScalarParam(INT_0, "Thickness");
        pShader->AddScalarParam(VEC4_0, "OutLine Color");

        pShader->SetName(L"2D_OutLineShader");
        AddAsset(L"2D_OutLineShader", pShader);
    }

    // =================================
    // 3D OutLine Shader
    // =================================
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
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\SkyBoxVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\SkyboxPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEXCUBE_0, "CubeMap Texture");

        pShader->SetName(L"SkyBoxShader");
        AddAsset(L"SkyBoxShader", pShader);
    }

    // =================================
    // BillBoardPoint Shader
    // =================================
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

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"BillBoardPointShader");
        AddAsset(L"BillBoardPointShader", pShader);
    }

    // =================================
    // IDMap Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\IDMapVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\IDMapPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS_EQUAL);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddScalarParam(VEC4_0, "Picking Color");

        pShader->SetName(L"IDMapShader");
        AddAsset(L"IDMapShader", pShader);
    }

    // =================================
    // Combine Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\Combine.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(FLOAT_0, "Strength");
        pShader->AddScalarParam(FLOAT_1, "Threshold");

        pShader->AddTexParam(TEX_0, "Render Texture");
        pShader->AddTexParam(TEX_1, "Bloom Texture");

        pShader->SetName(L"CombineShader");
        AddAsset(L"CombineShader", pShader);
    }

    // =================================
    // BloomDown Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BloomDownPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddTexParam(TEX_0, "Bloom Texture");

        pShader->SetName(L"BloomDownShader");
        AddAsset(L"BloomDownShader", pShader);
    }

    // =================================
    // BloomUp Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BloomUpPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddTexParam(TEX_0, "Bloom Texture");

        pShader->SetName(L"BloomUpShader");
        AddAsset(L"BloomUpShader", pShader);
    }

    // =================================
    // BlurX Shader
    // =================================
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
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\ShockWaveVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ShockWavePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(FLOAT_0, "Circle Size");

        pShader->SetName(L"ShockWaveShader");
        AddAsset(L"ShockWaveShader", pShader);
    }

    // =================================
    // Tone Mapping Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ToneMappingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(FLOAT_0, "Exposure"); // 렌즈를 오래 열어두면 빛을 많이 받아 들이는 것을 수치적으로 따라한 것
        pShader->AddScalarParam(FLOAT_1, "Gamma");          // 어떤 영역의 색을 더 넓게 보여줄지 의미함
        pShader->AddScalarParam(FLOAT_2, "Bloom Strength"); // Bloom Strength

        pShader->AddTexParam(TEX_0, "Render Texture");
        pShader->AddTexParam(TEX_1, "Bloom Texture");

        pShader->SetName(L"ToneMappingShader");
        AddAsset(L"ToneMappingShader", pShader);
    }

    // =================================
    // Depth Only Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\DepthOnlyVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\DepthOnlyPS.hlsl", "main");

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"DepthOnlyShader");
        AddAsset(L"DepthOnlyShader", pShader);
    }

    // =================================
    // Post Effect Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\PostEffectPS.hlsl", "main");

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(INT_0, "Render Mode");

        pShader->AddScalarParam(FLOAT_0, "FogStrength");
        pShader->AddScalarParam(FLOAT_1, "DepthScale");

        pShader->AddScalarParam(VEC4_0, "Fog Color");

        pShader->AddTexParam(TEX_0, "Render Texture");
        pShader->AddTexParam(TEX_1, "DepthOnly Texture");

        pShader->SetName(L"PostEffectShader");
        AddAsset(L"PostEffectShader", pShader);
    }
}

void CAssetMgr::CreateDefaultComputeShader()
{
    // =================================
    // SetColor Shader
    // =================================
    {
        Ptr<CComputeShader> pShader = new CSetColorShader;
        pShader->SetName(L"SetColorShader");
        AddAsset(L"SetColorShader", pShader);
    }

    // =================================
    // SetColor Shader
    // =================================
    {
        Ptr<CComputeShader> pShader = new CParticleUpdate;
        pShader->SetName(L"ParticleUpdateShader");
        AddAsset(L"ParticleUpdateShader", pShader);
    }
}

void CAssetMgr::CreateDefaultMaterial()
{
    // Std2DMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
        pMtrl->SetName(L"Std2DMtrl");
        AddAsset<CMaterial>(L"Std2DMtrl", pMtrl);
    }

    // Std2DLightMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DLightShader"));
        pMtrl->SetName(L"Std2DLightMtrl");
        AddAsset<CMaterial>(L"Std2DLightMtrl", pMtrl);
    }

    // TileMapMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"TileMapShader"));
        pMtrl->SetName(L"TileMapMtrl");
        AddAsset<CMaterial>(L"TileMapMtrl", pMtrl);
    }

    // ParticleRenderMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ParticleRenderShader"));
        pMtrl->SetName(L"ParticleRenderMtrl");
        AddAsset<CMaterial>(L"ParticleRenderMtrl", pMtrl);
    }

    // GrayFilterMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"GrayFilterShader"));
        pMtrl->SetName(L"GrayFilterMtrl");
        AddAsset<CMaterial>(L"GrayFilterMtrl", pMtrl);
    }

    // BlendFilterMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlendFilterShader"));
        pMtrl->SetName(L"BlendFilterMtrl");
        AddAsset<CMaterial>(L"BlendFilterMtrl", pMtrl);
    }

    // DistortionMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DistortionShader"));
        pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"Texture\\noise\\noise_01.jpg"));
        pMtrl->SetName(L"DistortionMtrl");
        AddAsset<CMaterial>(L"DistortionMtrl", pMtrl);
    }

    // DebugShapeMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DebugShapeShader"));
        pMtrl->SetName(L"DebugShapeMtrl");
        AddAsset<CMaterial>(L"DebugShapeMtrl", pMtrl);
    }

    // Unreal PBR
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRShader"));
        pMtrl->SetScalarParam(FLOAT_0, 1.f); // HeightScale
        pMtrl->SetScalarParam(INT_0, 0);     // Invert NormalMap Y

        pMtrl->SetName(L"UnrealPBRMtrl");
        AddAsset<CMaterial>(L"UnrealPBRMtrl", pMtrl);
    }

    // BlinnPhong
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlinnPhongShader"));
        pMtrl->SetMaterialCoefficient(Vec4(), Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4(0.5f, 0.5f, 0.5f, 1.f), 0.f, 0.f, Vec4());
        pMtrl->SetName(L"BlinnPhongMtrl");
        AddAsset<CMaterial>(L"BlinnPhongMtrl", pMtrl);
    }

    // NormalLine
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"NormalLineShader"));
        pMtrl->SetName(L"NormalLineMtrl");
        AddAsset<CMaterial>(L"NormalLineMtrl", pMtrl);
    }

    // 2D OutLine
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"2D_OutLineShader"));
        pMtrl->SetScalarParam(INT_0, 10);                                                     // Thickness
        pMtrl->SetScalarParam(VEC4_0, Vec4(235.f / 255.f, 162.f / 255.f, 10.f / 255.f, 1.f)); // Color
        pMtrl->SetName(L"2D_OutLineMtrl");
        AddAsset<CMaterial>(L"2D_OutLineMtrl", pMtrl);
    }

    // 3D OutLine
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"3D_OutLineShader"));
        pMtrl->SetName(L"3D_OutLineMtrl");
        AddAsset<CMaterial>(L"3D_OutLineMtrl", pMtrl);
    }

    // IDMap
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"IDMapShader"));
        pMtrl->SetName(L"IDMapMtrl");
        AddAsset<CMaterial>(L"IDMapMtrl", pMtrl);
    }

    // SkyBox CubeMap
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SkyBoxShader"));
        pMtrl->SetName(L"SkyBoxMtrl");
        AddAsset<CMaterial>(L"SkyBoxMtrl", pMtrl);
    }

    // Directional Light
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\DirectionalLight.png", L"Icons\\DirectionalLight.png"));
        pMtrl->SetName(L"DirectionalLightMtrl");
        AddAsset<CMaterial>(L"DirectionalLightMtrl", pMtrl);
    }

    // Point Light
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\PointLight.png", L"Icons\\PointLight.png"));
        pMtrl->SetName(L"PointLightMtrl");
        AddAsset<CMaterial>(L"PointLightMtrl", pMtrl);
    }

    // Spot Light
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\SpotLight.png", L"Icons\\SpotLight.png"));
        pMtrl->SetName(L"SpotLightMtrl");
        AddAsset<CMaterial>(L"SpotLightMtrl", pMtrl);
    }

    // Combine
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"CombineShader"));
        pMtrl->SetScalarParam(FLOAT_0, 1.f);  // Strength
        pMtrl->SetScalarParam(FLOAT_1, 0.5f); // Threshold
        pMtrl->SetName(L"CombineMtrl");
        AddAsset<CMaterial>(L"CombineMtrl", pMtrl);
    }

    // Bloom Down
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BloomDownShader"));
        pMtrl->SetName(L"BloomDownMtrl");
        AddAsset<CMaterial>(L"BloomDownMtrl", pMtrl);
    }

    // Bloom Up
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BloomUpShader"));
        pMtrl->SetName(L"BloomUpMtrl");
        AddAsset<CMaterial>(L"BloomUpMtrl", pMtrl);
    }

    // BlurX
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlurXShader"));
        pMtrl->SetName(L"BlurXMtrl");
        AddAsset<CMaterial>(L"BlurXMtrl", pMtrl);
    }

    // BlurY
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlurYShader"));
        pMtrl->SetName(L"BlurYMtrl");
        AddAsset<CMaterial>(L"BlurYMtrl", pMtrl);
    }

    // ShockWave
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ShockWaveShader"));
        pMtrl->SetName(L"ShockWaveMtrl");
        AddAsset<CMaterial>(L"ShockWaveMtrl", pMtrl);
    }

    // ToneMapping
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ToneMappingShader"));
        pMtrl->SetScalarParam(FLOAT_0, 1.f);  // Exposure
        pMtrl->SetScalarParam(FLOAT_1, 2.2f); // Gamma
        pMtrl->SetName(L"ToneMappingMtrl");
        AddAsset<CMaterial>(L"ToneMappingMtrl", pMtrl);
    }

    // DepthOnly
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DepthOnlyShader"));
        pMtrl->SetName(L"DepthOnlyMtrl");
        AddAsset<CMaterial>(L"DepthOnlyMtrl", pMtrl);
    }

    // Post Effect
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"PostEffectShader"));
        pMtrl->SetName(L"PostEffectMtrl");
        pMtrl->SetScalarParam(FLOAT_1, 1.f);
        pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
        AddAsset<CMaterial>(L"PostEffectMtrl", pMtrl);
    }
}

void CAssetMgr::CreateDefaultPhysics2DMaterial()
{
    // Default Material
    {
        Ptr<CPhysics2DMaterial> pMtrl = new CPhysics2DMaterial(true);
        pMtrl->SetFriction(0.6f);
        pMtrl->SetBounciness(0.f);
        AddAsset<CPhysics2DMaterial>(L"Default Material", pMtrl);
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

            v.vTangent = biTangent.Cross(normalOrth);
            v.vTangent.Normalize();

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

    vector<Vec3> pos = {Vec3(-X, 0.0f, Z), Vec3(X, 0.0f, Z),   Vec3(-X, 0.0f, -Z), Vec3(X, 0.0f, -Z), Vec3(0.0f, Z, X),  Vec3(0.0f, Z, -X),
                        Vec3(0.0f, -Z, X), Vec3(0.0f, -Z, -X), Vec3(Z, X, 0.0f),   Vec3(-Z, X, 0.0f), Vec3(Z, -X, 0.0f), Vec3(-Z, -X, 0.0f)};

    for (size_t i = 0; i < pos.size(); i++)
    {
        Vtx v;
        v.vPos = pos[i];
        v.vNormal = v.vPos;
        v.vNormal.Normalize();

        newMesh.vertices.push_back(v);
    }

    newMesh.indices = {1, 4,  0, 4,  9, 0, 4, 5,  9, 8, 5, 4,  1, 8, 4, 1,  10, 8, 10, 3, 8, 8, 3,  5, 3, 2, 5, 3,  7, 2,
                       3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1,  6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7};

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

tMeshData CAssetMgr::MakeWireBox(const Vector3 center, const Vector3 extents)
{
    // 상자를 와이어 프레임으로 그리는 용도
    vector<Vector3> positions;

    // 앞면
    positions.push_back(center + Vector3(-1.0f, -1.0f, -1.0f) * extents);
    positions.push_back(center + Vector3(-1.0f, 1.0f, -1.0f) * extents);
    positions.push_back(center + Vector3(1.0f, 1.0f, -1.0f) * extents);
    positions.push_back(center + Vector3(1.0f, -1.0f, -1.0f) * extents);

    // 뒷면
    positions.push_back(center + Vector3(-1.0f, -1.0f, 1.0f) * extents);
    positions.push_back(center + Vector3(-1.0f, 1.0f, 1.0f) * extents);
    positions.push_back(center + Vector3(1.0f, 1.0f, 1.0f) * extents);
    positions.push_back(center + Vector3(1.0f, -1.0f, 1.0f) * extents);

    tMeshData meshData;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vNormal = positions[i] - center;
        v.vNormal.Normalize();
        v.vUV = Vector2(0.0f); // 미사용
        meshData.vertices.push_back(v);
    }

    // Line list
    meshData.indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // 앞면
        4, 5, 5, 6, 6, 7, 7, 4, // 뒷면
        0, 4, 1, 5, 2, 6, 3, 7  // 옆면
    };

    return meshData;
}

tMeshData CAssetMgr::MakeWireSphere(const Vector3 center, const float radius)
{
    tMeshData meshData;
    vector<Vtx>& vertices = meshData.vertices;
    vector<uint32_t>& indices = meshData.indices;

    const int numPoints = 30;
    const float dTheta = XM_2PI / float(numPoints);

    // XY plane
    {
        int offset = int(vertices.size());
        Vector3 start(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < numPoints; i++)
        {
            Vtx v;
            v.vPos = center + Vector3::Transform(start, Matrix::CreateRotationZ(dTheta * float(i))) * radius;
            vertices.push_back(v);
            indices.push_back(i + offset);
            if (i != 0)
            {
                indices.push_back(i + offset);
            }
        }
        indices.push_back(offset);
    }

    // YZ
    {
        int offset = int(vertices.size());
        Vector3 start(0.0f, 1.0f, 0.0f);
        for (int i = 0; i < numPoints; i++)
        {
            Vtx v;
            v.vPos = center + Vector3::Transform(start, Matrix::CreateRotationX(dTheta * float(i))) * radius;
            vertices.push_back(v);
            indices.push_back(i + offset);
            if (i != 0)
            {
                indices.push_back(i + offset);
            }
        }
        indices.push_back(offset);
    }

    // XZ
    {
        int offset = int(vertices.size());
        Vector3 start(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < numPoints; i++)
        {
            Vtx v;
            v.vPos = center + Vector3::Transform(start, Matrix::CreateRotationY(dTheta * float(i))) * radius;
            vertices.push_back(v);
            indices.push_back(i + offset);
            if (i != 0)
            {
                indices.push_back(i + offset);
            }
        }
        indices.push_back(offset);
    }

    // for (auto &v : vertices) {
    //     cout << v.position.x << " " << v.position.y << " " << v.position.z
    //          << endl;
    // }

    // for (int i = 0; i < indices.size(); i++) {
    //     cout << indices[i] << " ";
    // }
    // cout << endl;

    return meshData;
}
