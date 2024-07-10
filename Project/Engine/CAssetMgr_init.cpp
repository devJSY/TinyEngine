#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"

#include "CSetColorShader.h"
#include "CParticleUpdate.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"
#include "CAnimationUpdateShader.h"
#include "CCopyBoneShader.h"

void CAssetMgr::CreateDefaultMesh()
{
    // Point
    {
        auto mesh = MakePoint();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"PointMesh");
        AddAsset(L"PointMesh", pMesh);
    }

    // Crosshair
    {
        auto mesh = MakeCrosshair();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CrosshairMesh");
        AddAsset(L"CrosshairMesh", pMesh);
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
        auto mesh = MakeSphere(1, 25, 25);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"SphereMesh");
        AddAsset(L"SphereMesh", pMesh);
    }

    // Capsule
    {
        auto mesh = MakeCapsule(0.5f, 0.5f, 30);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"CapsuleMesh");
        AddAsset(L"CapsuleMesh", pMesh);
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

    // Cone
    {
        auto mesh = MakeCone(0.5f, 1.f);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"ConeMesh");
        AddAsset(L"ConeMesh", pMesh);
    }

    // Wire Circle
    {
        auto mesh = MakeWireCircle(1.f, 30);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireCircleMesh");
        AddAsset(L"WireCircleMesh", pMesh);
    }

    // Wire Box
    {
        auto mesh = MakeWireBox();
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireBoxMesh");
        AddAsset(L"WireBoxMesh", pMesh);
    }

    // Wire Sphere
    {
        auto mesh = MakeWireSphere(1.f, 30);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireSphereMesh");
        AddAsset(L"WireSphereMesh", pMesh);
    }

    // Wire Capsule 2D
    {
        auto mesh = MakeWireCapsule2D(0.5f, 0.5f, 15);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireCapsuleMesh2D");
        AddAsset(L"WireCapsuleMesh2D", pMesh);
    }

    // Wire Capsule
    {
        auto mesh = MakeWireCapsule(0.5f, 0.5f, 15);
        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(mesh.vertices.data(), (UINT)mesh.vertices.size(), mesh.indices.data(), (UINT)mesh.indices.size());
        pMesh->SetName(L"WireCapsuleMesh");
        AddAsset(L"WireCapsuleMesh", pMesh);
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
    // UIHPShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIHPShaderVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIHPShaderPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"UIHPShader");
        AddAsset(L"UIHPShader", pShader);
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
    // Std2DEffectShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Effect");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddTexParam(TEX_0, "Texture");
        pShader->AddScalarParam(INT_0, "Glow Enable");
        pShader->AddScalarParam(FLOAT_0, "Glow Threshold", 0.1f);
        pShader->AddScalarParam(VEC4_0, "Glow Color");

        pShader->AddScalarParam(INT_1, "Use Albedo");

        pShader->SetName(L"Std2DEffectShader");
        AddAsset(L"Std2DEffectShader", pShader);
    }

    // =================================
    // Std2DGlowShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Glow");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");
        pShader->AddScalarParam(INT_0, "Glow Enable");
        pShader->AddScalarParam(FLOAT_0, "Glow Threshold");
        pShader->AddScalarParam(VEC4_0, "Glow Color");

        pShader->SetName(L"Std2DGlowShader");
        AddAsset(L"Std2DGlowShader", pShader);
    }

    // =================================
    // Std3DShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
        pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Ambient Texture");
        pShader->AddTexParam(TEX_1, "Normal Texture");

        pShader->SetName(L"Std3DShader");
        AddAsset(L"Std3DShader", pShader);
    }

    // ====================
    // Std3D_DeferredShader
    // ====================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
        pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Ambient Texture");
        pShader->AddTexParam(TEX_1, "Normal Texture");

        pShader->SetName(L"Std3D_DeferredShader");
        AddAsset(L"Std3D_DeferredShader", pShader);
    }

    // ====================
    // DirLight_deferredShader
    // ====================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\light_deferred.fx", "VS_DirLight");
        pShader->CreatePixelShader(L"shader\\light_deferred.fx", "PS_DirLight");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ONE_ONE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

        pShader->SetName(L"DirLight_deferredShader");
        AddAsset(L"DirLight_deferredShader", pShader);
    }

    // ====================
    // PointLight_deferredShader
    // ====================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\light_deferred.fx", "VS_PointLight");
        pShader->CreatePixelShader(L"shader\\light_deferred.fx", "PS_PointLight");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ONE_ONE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

        pShader->SetName(L"PointLight_deferredShader");
        AddAsset(L"PointLight_deferredShader", pShader);
    }

    // ====================
    // SpotLight_deferredShader
    // ====================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\light_deferred.fx", "VS_SpotLight");
        pShader->CreatePixelShader(L"shader\\light_deferred.fx", "PS_SpotLight");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ONE_ONE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

        pShader->SetName(L"SpotLight_deferredShader");
        AddAsset(L"SpotLight_deferredShader", pShader);
    }

    // ====================
    // Merge Deferred Shader
    // ====================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\merge_deferred.fx", "VS_Merge");
        pShader->CreatePixelShader(L"shader\\merge_deferred.fx", "PS_Merge");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MERGE);

        pShader->SetName(L"Merge_DeferredShader");
        AddAsset(L"Merge_DeferredShader", pShader);
    }

    // ============
    // Decal Shader
    // ============
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
        pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::DECAL);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

        pShader->SetName(L"DecalShader");
        AddAsset(L"DecalShader", pShader);
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
        pShader->AddScalarParam(VEC2_0, "Slice UV", 0.1f);
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
    // ParticleRender Glow
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\particle_render.fx", "VS_ParticleRender");
        pShader->CreateGeometryShader(L"shader\\particle_render.fx", "GS_ParticleRender");
        pShader->CreatePixelShader(L"shader\\particle_render.fx", "PS_ParticleRender_Glow");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        pShader->AddTexParam(TEX_0, "Texture");
        pShader->AddScalarParam(INT_0, "Glow Enable");
        pShader->AddScalarParam(FLOAT_0, "Glow Threshold", 0.1f);
        pShader->AddScalarParam(VEC4_0, "Glow Color");

        pShader->SetName(L"ParticleRenderGlowShader");
        AddAsset(L"ParticleRenderGlowShader", pShader);
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
    // CinematicFilter Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Postprocess");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_CinematicFilter");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(INT_0, "Cinematic Enable");
        pShader->AddScalarParam(FLOAT_0, "Thickness", 0.1f);
        pShader->AddScalarParam(VEC4_0, "Cinematic Color");

        pShader->SetName(L"CinematicFilterShader");
        AddAsset(L"CinematicFilterShader", pShader);
    }

    // =================================
    // AnimationFilter Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Postprocess");
        pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_AnimationFilter");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"AnimationFilterShader");
        AddAsset(L"AnimationFilterShader", pShader);
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

    // ===========
    // Tess Shader
    // ===========
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\tess.fx", "VS_Tess");
        pShader->CreateHullShader(L"shader\\tess.fx", "HS_Tess");
        pShader->CreateDomainShader(L"shader\\tess.fx", "DS_Tess");
        pShader->CreatePixelShader(L"shader\\tess.fx", "PS_Tess");

        pShader->SetRSType(RS_TYPE::WIRE_FRAME);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
        pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

        pShader->AddScalarParam(VEC4_0, "Tessellation Factor", 0.1f);

        pShader->SetName(L"TessShader");
        AddAsset(L"TessShader", pShader);
    }

    // ================
    // LandScape Shader
    // ================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\landscape.fx", "VS_LandScape");
        pShader->CreateHullShader(L"shader\\landscape.fx", "HS_LandScape");
        pShader->CreateDomainShader(L"shader\\landscape.fx", "DS_LandScape");
        pShader->CreatePixelShader(L"shader\\landscape.fx", "PS_LandScape");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
        pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

        pShader->AddScalarParam(FLOAT_1, "Tessellation Distance Scale");

        pShader->AddTexParam(TEX_0, "Ambient Texture");
        pShader->AddTexParam(TEX_1, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_4, "Emissive Texture");

        pShader->SetName(L"LandScapeShader");
        AddAsset(L"LandScapeShader", pShader);
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
        pShader->AddScalarParam(FLOAT_0, "HeightMapping Scale", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");

        pShader->AddTexParam(TEX_0, "Ambient Texture");
        pShader->AddTexParam(TEX_1, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_2, "Normal Texture");
        pShader->AddTexParam(TEX_3, "Height Texture");
        pShader->AddTexParam(TEX_4, "Emissive Texture");

        pShader->SetName(L"UnrealPBRShader");
        AddAsset(L"UnrealPBRShader", pShader);
    }

    // =================================
    // Unreal PBR Deferred Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UnrealPBRDeferredPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddScalarParam(INT_0, "Invert NormalMapY");
        pShader->AddScalarParam(FLOAT_0, "HeightMapping Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Ambient Texture");
        pShader->AddTexParam(TEX_1, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_2, "Normal Texture");
        pShader->AddTexParam(TEX_3, "Height Texture");
        pShader->AddTexParam(TEX_4, "Emissive Texture");

        pShader->SetName(L"UnrealPBRDeferredShader");
        AddAsset(L"UnrealPBRDeferredShader", pShader);
    }

    // =================================
    // Unreal PBR Deferred Dir Lighting Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRDeferredDirLightingVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UnrealPBRDeferredDirLightingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ONE_ONE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

        pShader->SetName(L"UnrealPBRDeferredDirLightingShader");
        AddAsset(L"UnrealPBRDeferredDirLightingShader", pShader);
    }

    // =================================
    // Unreal PBR Deferred Point Lighting Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRDeferredPointLightingVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UnrealPBRDeferredPointLightingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ONE_ONE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

        pShader->SetName(L"UnrealPBRDeferredPointLightingShader");
        AddAsset(L"UnrealPBRDeferredPointLightingShader", pShader);
    }

    // =================================
    // Unreal PBR Deferred Spot Lighting Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRDeferredSpotLightingVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UnrealPBRDeferredSpotLightingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ONE_ONE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

        pShader->SetName(L"UnrealPBRDeferredSpotLightingShader");
        AddAsset(L"UnrealPBRDeferredSpotLightingShader", pShader);
    }

    // =================================
    // Unreal PBR Deferred Merge Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRDeferredMergeVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UnrealPBRDeferredMergePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MERGE);

        pShader->SetName(L"UnrealPBRDeferredMergeShader");
        AddAsset(L"UnrealPBRDeferredMergeShader", pShader);
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

        pShader->AddScalarParam(INT_0, "Thickness", 0.1f);
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
        pShader->CreateVertexShader(L"shader\\skybox.fx", "VS_SkyBox");
        pShader->CreatePixelShader(L"shader\\skybox.fx", "PS_SkyBox");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::LESS_EQUAL);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEXCUBE_0, "CubeMap Texture");

        pShader->SetName(L"SkyBoxShader");
        AddAsset(L"SkyBoxShader", pShader);
    }

    // =================================
    // SkyBox Shader Version 2
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

        pShader->SetName(L"SkyBoxShader_V2");
        AddAsset(L"SkyBoxShader_V2", pShader);
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
        pShader->AddScalarParam(FLOAT_0, "Half Width", 0.1f);

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
    // Sampling Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\SamplingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(FLOAT_0, "Threshold", 0.01f);

        pShader->AddTexParam(TEX_0, "Sampling Texture");

        pShader->SetName(L"SamplingShader");
        AddAsset(L"SamplingShader", pShader);
    }

    // =================================
    // Combine Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\CombinePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(FLOAT_0, "Strength", 0.01f);

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

        pShader->AddScalarParam(FLOAT_0, "Circle Size", 0.1f);

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

        pShader->AddScalarParam(FLOAT_0, "Exposure", 0.01f); // 렌즈를 오래 열어두면 빛을 많이 받아 들이는 것을 수치적으로 따라한 것
        pShader->AddScalarParam(FLOAT_1, "Gamma", 0.01f);          // 어떤 영역의 색을 더 넓게 보여줄지 의미함
        pShader->AddScalarParam(FLOAT_2, "Bloom Strength", 0.01f); // Bloom Strength

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

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_SHADOW);

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

        pShader->AddScalarParam(FLOAT_0, "FogStrength", 0.01f);
        pShader->AddScalarParam(FLOAT_1, "FogScale", 0.01f);
        pShader->AddScalarParam(FLOAT_2, "DepthScale", 0.01f);

        pShader->AddScalarParam(VEC4_0, "Fog Color");

        pShader->AddTexParam(TEX_0, "Render Texture");
        pShader->AddTexParam(TEX_1, "DepthOnly Texture");

        pShader->SetName(L"PostEffectShader");
        AddAsset(L"PostEffectShader", pShader);
    }

    // =================================
    // ProgressBar Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\ProgressBarVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ProgressBarPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEX_0, "Render Texture");

        pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "Max Value");
        pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "Cur Value");
        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Color");

        pShader->SetName(L"ProgressBarShader");
        AddAsset(L"ProgressBarShader", pShader);
    }

    // =================================
    // Color Grading Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ColorGradingPS.hlsl", "main");

        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddTexParam(TEX_0, "LUT Texture");

        pShader->SetName(L"ColorGradingShader");
        AddAsset(L"ColorGradingShader", pShader);
    }

    // =================================
    // SSAO Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\SSAOPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->AddTexParam(TEX_0, "DepthOnly Texture");
        pShader->AddScalarParam(FLOAT_0, "Sample Radius", 1e-3f);
        pShader->AddScalarParam(FLOAT_1, "Pow Power", 1e-3f);

        pShader->SetName(L"SSAOShader");
        AddAsset(L"SSAOShader", pShader);
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
    // Particle Update Shader
    // =================================
    {
        Ptr<CComputeShader> pShader = new CParticleUpdate;
        pShader->SetName(L"ParticleUpdateShader");
        AddAsset(L"ParticleUpdateShader", pShader);
    }

    // ======================
    // 높이 수정 컴퓨트 쉐이더
    // ======================
    {
        Ptr<CComputeShader> pShader = new CHeightMapShader;
        pShader->SetName(L"HeightMapShader");
        AddAsset<CComputeShader>(L"HeightMapShader", pShader);
    }

    // =====================
    // 지형 피킹 컴퓨트 쉐이더
    // =====================
    {
        Ptr<CComputeShader> pShader = new CRaycastShader;
        pShader->SetName(L"RaycastShader");
        AddAsset<CComputeShader>(L"RaycastShader", pShader);
    }

    // =======================
    // 가중치 수정 컴퓨트 쉐이더
    // =======================
    {
        Ptr<CComputeShader> pShader = new CWeightMapShader;
        pShader->SetName(L"WeightMapShader");
        AddAsset<CComputeShader>(L"WeightMapShader", pShader);
    }

    // =======================
    // Animation Update
    // =======================
    {
        Ptr<CComputeShader> pShader = new CAnimationUpdateShader;
        pShader->SetName(L"AnimationUpdateCS");
        AddAsset<CComputeShader>(L"AnimationUpdateCS", pShader);
    }

    // =======================
    // CopyBone
    // =======================
    {
        Ptr<CComputeShader> pShader = new CCopyBoneShader;
        pShader->SetName(L"CopyBoneCS");
        AddAsset<CComputeShader>(L"CopyBoneCS", pShader);
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

    // Std2DEffectMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DEffectShader"));
        pMtrl->SetName(L"Std2DEffectMtrl");
        AddAsset<CMaterial>(L"Std2DEffectMtrl", pMtrl);
    }

    // Std2DGlowMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DGlowShader"));
        pMtrl->SetName(L"Std2DGlowMtrl");
        AddAsset<CMaterial>(L"Std2DGlowMtrl", pMtrl);
    }

    // Std3DMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std3DShader"));
        pMtrl->SetMaterialCoefficient(Vec4(), Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4(0.5f, 0.5f, 0.5f, 1.f), 0.f, 0.f, Vec4());
        pMtrl->SetName(L"Std3DMtrl");
        AddAsset<CMaterial>(L"Std3DMtrl", pMtrl);
    }

    // Std3D_DeferredMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std3D_DeferredShader"));
        pMtrl->SetMaterialCoefficient(Vec4(), Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4(0.5f, 0.5f, 0.5f, 1.f), 0.f, 0.f, Vec4());
        pMtrl->SetName(L"Std3D_DeferredMtrl");
        AddAsset<CMaterial>(L"Std3D_DeferredMtrl", pMtrl);
    }

    // DirLight_deferredMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DirLight_deferredShader"));
        pMtrl->SetName(L"DirLight_deferredMtrl");
        AddAsset<CMaterial>(L"DirLight_deferredMtrl", pMtrl);
    }

    // PointLight_deferredMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"PointLight_deferredShader"));
        pMtrl->SetName(L"PointLight_deferredMtrl");
        AddAsset<CMaterial>(L"PointLight_deferredMtrl", pMtrl);
    }

    // SpotLight_deferredMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SpotLight_deferredShader"));
        pMtrl->SetName(L"SpotLight_deferredMtrl");
        AddAsset<CMaterial>(L"SpotLight_deferredMtrl", pMtrl);
    }

    // Merge_DeferredMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Merge_DeferredShader"));
        pMtrl->SetName(L"Merge_DeferredMtrl");
        AddAsset<CMaterial>(L"Merge_DeferredMtrl", pMtrl);
    }

    // DecalMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DecalShader"));
        AddAsset<CMaterial>(L"DecalMtrl", pMtrl);
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

    // ParticleRenderGlowMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ParticleRenderGlowShader"));
        pMtrl->SetName(L"ParticleRenderGlowMtrl");
        AddAsset<CMaterial>(L"ParticleRenderGlowMtrl", pMtrl);
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

    // CinematicFilterMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"CinematicFilterShader"));
        pMtrl->SetScalarParam(INT_0, 1);
        pMtrl->SetScalarParam(FLOAT_0, 0.15f);
        pMtrl->SetScalarParam(VEC4_0, Vec4(0.f, 0.f, 0.f, 1.f));

        pMtrl->SetName(L"CinematicFilterMtrl");
        AddAsset<CMaterial>(L"CinematicFilterMtrl", pMtrl);
    }

    // AnimationFilterMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"AnimationFilterShader"));
        pMtrl->SetName(L"AnimationFilterMtrl");
        AddAsset<CMaterial>(L"AnimationFilterMtrl", pMtrl);
    }

    // DebugShapeMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DebugShapeShader"));
        pMtrl->SetName(L"DebugShapeMtrl");
        AddAsset<CMaterial>(L"DebugShapeMtrl", pMtrl);
    }

    // TessMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"TessShader"));
        pMtrl->SetScalarParam(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f)); // TessFactor

        pMtrl->SetName(L"TessMtrl");
        AddAsset<CMaterial>(L"TessMtrl", pMtrl);
    }

    // LandScapeMtrl
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"LandScapeShader"));
        pMtrl->SetScalarParam(FLOAT_1, 1.f); // Tess Dist Scale

        pMtrl->SetName(L"LandScapeMtrl");
        AddAsset<CMaterial>(L"LandScapeMtrl", pMtrl);
    }

    // Unreal PBR
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRShader"));
        pMtrl->SetScalarParam(FLOAT_0, 0.f); // HeightScale
        pMtrl->SetScalarParam(INT_0, 0);     // Invert NormalMap Y

        pMtrl->SetName(L"UnrealPBRMtrl");
        AddAsset<CMaterial>(L"UnrealPBRMtrl", pMtrl);
    }

    // Unreal PBR Deferred
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRDeferredShader"));
        pMtrl->SetScalarParam(FLOAT_0, 0.f); // HeightScale
        pMtrl->SetScalarParam(INT_0, 0);     // Invert NormalMap Y

        pMtrl->SetName(L"UnrealPBRDeferredMtrl");
        AddAsset<CMaterial>(L"UnrealPBRDeferredMtrl", pMtrl);
    }

    // Unreal PBR Deferred Dir Lighting
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRDeferredDirLightingShader"));
        pMtrl->SetName(L"UnrealPBRDeferredDirLightingMtrl");
        AddAsset<CMaterial>(L"UnrealPBRDeferredDirLightingMtrl", pMtrl);
    }

    // Unreal PBR Deferred Point Lighting
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRDeferredPointLightingShader"));
        pMtrl->SetName(L"UnrealPBRDeferredPointLightingMtrl");
        AddAsset<CMaterial>(L"UnrealPBRDeferredPointLightingMtrl", pMtrl);
    }

    // Unreal PBR Deferred Spot Lighting
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRDeferredSpotLightingShader"));
        pMtrl->SetName(L"UnrealPBRDeferredSpotLightingMtrl");
        AddAsset<CMaterial>(L"UnrealPBRDeferredSpotLightingMtrl", pMtrl);
    }

    // Unreal PBR Deferred Merge
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRDeferredMergeShader"));
        pMtrl->SetName(L"UnrealPBRDeferredMergeMtrl");
        AddAsset<CMaterial>(L"UnrealPBRDeferredMergeMtrl", pMtrl);
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

    // SkyBox CubeMap Version 2
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SkyBoxShader_V2"));
        pMtrl->SetName(L"SkyBoxMtrl");
        AddAsset<CMaterial>(L"SkyBoxMtrl_V2", pMtrl);
    }

    // Directional Light
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\DirectionalLight.png", L"Icons\\DirectionalLight.png"));
        pMtrl->SetScalarParam(FLOAT_0, 0.5f); // HalfWidth
        pMtrl->SetName(L"DirectionalLightMtrl");
        AddAsset<CMaterial>(L"DirectionalLightMtrl", pMtrl);
    }

    // Point Light
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\PointLight.png", L"Icons\\PointLight.png"));
        pMtrl->SetScalarParam(FLOAT_0, 0.5f); // HalfWidth
        pMtrl->SetName(L"PointLightMtrl");
        AddAsset<CMaterial>(L"PointLightMtrl", pMtrl);
    }

    // Spot Light
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\SpotLight.png", L"Icons\\SpotLight.png"));
        pMtrl->SetScalarParam(FLOAT_0, 0.5f); // HalfWidth
        pMtrl->SetName(L"SpotLightMtrl");
        AddAsset<CMaterial>(L"SpotLightMtrl", pMtrl);
    }

    // Camera Icon
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BillBoardPointShader"));
        pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"Icons\\CameraIcon.png", L"Icons\\CameraIcon.png"));
        pMtrl->SetScalarParam(FLOAT_0, 0.5f); // HalfWidth
        pMtrl->SetName(L"CameraIconMtrl");
        AddAsset<CMaterial>(L"CameraIconMtrl", pMtrl);
    }

    // Sampling
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SamplingShader"));
        pMtrl->SetScalarParam(FLOAT_1, 0.5f); // Threshold
        pMtrl->SetName(L"SamplingMtrl");
        AddAsset<CMaterial>(L"SamplingMtrl", pMtrl);
    }

    // Combine
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"CombineShader"));
        pMtrl->SetScalarParam(FLOAT_0, 1.f); // Strength
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
        pMtrl->SetScalarParam(FLOAT_2, 0.5f); //  Bloom Strength
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

    // ProgressBar
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ProgressBarShader"));
        pMtrl->SetName(L"ProgressBarMtrl");
        pMtrl->SetScalarParam(FLOAT_0, 1.f);
        pMtrl->SetScalarParam(FLOAT_1, 1.f);
        AddAsset<CMaterial>(L"ProgressBarMtrl", pMtrl);
    }

    // ColorGrading
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ColorGradingShader"));
        pMtrl->SetName(L"ColorGradingMtrl");
        AddAsset<CMaterial>(L"ColorGradingMtrl", pMtrl);
    }

    // SSAO
    {
        Ptr<CMaterial> pMtrl = new CMaterial(true);
        pMtrl->SetShader(FindAsset<CGraphicsShader>(L"SSAOShader"));
        pMtrl->SetName(L"SSAOMtrl");
        pMtrl->SetScalarParam(FLOAT_0, 0.5f); // Radius
        pMtrl->SetScalarParam(FLOAT_1, 1.f);  // Pow Power
        AddAsset<CMaterial>(L"SSAOMtrl", pMtrl);
    }
}

void CAssetMgr::CreateDefaultPhysics2DMaterial()
{
    // Default Material
    {
        Ptr<CPhysics2DMaterial> pMtrl = new CPhysics2DMaterial(true);
        AddAsset<CPhysics2DMaterial>(L"Default Material", pMtrl);
    }
}

void CAssetMgr::CreateDefaultPhysicMaterial()
{
    // Default Material
    {
        Ptr<CPhysicMaterial> pMtrl = new CPhysicMaterial(true);
        AddAsset<CPhysicMaterial>(L"Default Material", pMtrl);
    }
}

Ptr<CMeshData> CAssetMgr::LoadFBX(const wstring& _strPath)
{
    wstring strFileName = std::filesystem::path(_strPath).stem();

    wstring strName = L"meshdata\\";
    strName += strFileName + L".mdat";

    Ptr<CMeshData> pMeshData = FindAsset<CMeshData>(strName);

    if (nullptr != pMeshData)
        return pMeshData;

    pMeshData = CMeshData::LoadFromFBX(_strPath);
    pMeshData->SetName(strName);
    pMeshData->SetKey(strName);
    pMeshData->SetRelativePath(strName);

    m_mapAsset[(UINT)ASSET_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.Get()));

    // meshdata 를 파일 저장
    pMeshData->Save(strName);

    return pMeshData;
}

tMeshData CAssetMgr::MakePoint()
{
    tMeshData meshData;
    Vtx v;
    v.vPos = Vec3(0.f, 0.f, 0.f);
    v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    v.vUV = Vec2(0.5f, 0.5f);

    v.vNormal = Vec3(0.f, 0.f, -1.f);
    v.vTangent = Vec3(1.f, 0.f, 0.f);
    v.vNormal.Cross(v.vTangent, v.vBiTangent);
    v.vBiTangent.Normalize();

    meshData.vertices.push_back(v);
    meshData.indices.push_back(0);

    return meshData;
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
        v.vColor = colors[i];
        v.vColor.w = 1.f;
        v.vUV = texcoords[i];

        v.vNormal = Vec3(0.f, 0.f, -1.f);
        v.vTangent = Vec3(1.f, 0.f, 0.f);
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();

        meshData.vertices.push_back(v);
    }

    meshData.indices.push_back(0);
    meshData.indices.push_back(1);
    meshData.indices.push_back(2);
    meshData.indices.push_back(3);

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
        v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
        v.vUV = Vec2(cosf(fTheta), sinf(fTheta));

        v.vNormal = Vec3(0.f, 0.f, -1.f);
        v.vTangent = Vec3(1.f, 0.f, 0.f);
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();

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
    vector<Vec3> tangents;

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
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));

    tMeshData meshData;

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vColor = colors[i];
        v.vColor.w = 1.f;
        v.vUV = texcoords[i] * texScale;

        v.vNormal = normals[i];
        v.vTangent = tangents[i];
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();

        meshData.vertices.push_back(v);
    }
    meshData.indices = {
        0, 2, 3, 2, 0, 1, // 앞면
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
        v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
        v.vUV = Vec2(cosf(fTheta), sinf(fTheta));

        v.vNormal = Vec3(0.f, 0.f, -1.f);
        v.vTangent = Vec3(1.f, 0.f, 0.f);
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();

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
    vector<Vec3> tangents;

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
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));

    tMeshData meshData;

    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vColor = colors[i];
        v.vUV = texcoords[i] * texScale;
        v.vColor.w = 1.f;

        v.vNormal = normals[i];
        v.vTangent = tangents[i];
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();

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
            v.vUV = Vec2(x + 1.0f, y + 1.0f) * 0.5f * texScale;
            v.vNormal = Vec3(0.0f, 0.0f, -1.0f);
            v.vTangent = Vec3(1.0f, 0.0f, 0.0f);
            v.vNormal.Cross(v.vTangent, v.vBiTangent);
            v.vBiTangent.Normalize();

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
    vector<Vec3> tangents;

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
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));

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
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));

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
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));

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
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));
    tangents.push_back(Vec3(1.0f, 0.0f, 0.0f));

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
    tangents.push_back(Vec3(0.0f, 1.0f, 0.0f));
    tangents.push_back(Vec3(0.0f, 1.0f, 0.0f));
    tangents.push_back(Vec3(0.0f, 1.0f, 0.0f));
    tangents.push_back(Vec3(0.0f, 1.0f, 0.0f));

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
    tangents.push_back(Vec3(0.0f, -1.0f, 0.0f));
    tangents.push_back(Vec3(0.0f, -1.0f, 0.0f));
    tangents.push_back(Vec3(0.0f, -1.0f, 0.0f));
    tangents.push_back(Vec3(0.0f, -1.0f, 0.0f));

    tMeshData meshData;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vColor = colors[i];
        v.vColor.w = 1.f;
        v.vUV = texcoords[i];

        v.vNormal = normals[i];
        v.vTangent = tangents[i];
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();

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
        v.vUV = Vec2(float(i) / numSlices, 1.0f);

        // std::cout << v.vPos.x << " " << v.vPos.z << std::endl;

        v.vNormal = v.vPos - Vec3(0.0f, -0.5f * height, 0.0f);
        v.vNormal.Normalize();

        vertices.push_back(v);
    }

    // 옆면의 맨 위 버텍스들 (인덱스 numSlices 이상 2 * numSlices 미만)
    for (int i = 0; i <= numSlices; i++)
    {
        Vtx v;
        v.vPos = Vec3::Transform(Vec3(topRadius, 0.5f * height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));
        v.vUV = Vec2(float(i) / numSlices, 0.0f);

        v.vNormal = v.vPos - Vec3(0.0f, 0.5f * height, 0.0f);
        v.vNormal.Normalize();

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
            v.vNormal.Cross(v.vTangent, v.vBiTangent);
            v.vBiTangent.Normalize();

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

tMeshData CAssetMgr::MakeCapsule(const float radius, const float halfHeight, const int numSlices)
{
    tMeshData topSphere = MakeSphere(radius, numSlices, numSlices / 2);
    tMeshData bottomSphere = MakeSphere(radius, numSlices, numSlices / 2);

    // 상단과 하단을 y축 방향으로 이동시켜서 캡슐의 상단과 하단에 배치합니다.
    for (auto& vertex : topSphere.vertices)
        vertex.vPos.y += halfHeight;

    for (auto& vertex : bottomSphere.vertices)
        vertex.vPos.y -= halfHeight;

    // 이제 원기둥을 만듭니다.
    tMeshData cylinder = MakeCylinder(radius, radius, 2.0f * halfHeight, numSlices);

    // 상단과 하단, 그리고 원기둥의 메쉬 데이터를 결합하여 캡슐의 메쉬 데이터를 생성합니다.
    tMeshData capsule;
    capsule.vertices.insert(capsule.vertices.end(), topSphere.vertices.begin(), topSphere.vertices.end());
    capsule.vertices.insert(capsule.vertices.end(), bottomSphere.vertices.begin(), bottomSphere.vertices.end());
    capsule.vertices.insert(capsule.vertices.end(), cylinder.vertices.begin(), cylinder.vertices.end());

    // 상단과 하단, 원기둥의 인덱스를 조정하여 캡슐의 인덱스를 생성합니다.
    int offsetTop = int(topSphere.vertices.size());
    int offsetCylinder = int(offsetTop + bottomSphere.vertices.size());
    capsule.indices = topSphere.indices;
    for (auto& index : bottomSphere.indices)
        capsule.indices.push_back(index + offsetTop);
    for (auto& index : cylinder.indices)
        capsule.indices.push_back(index + offsetCylinder);

    return capsule;
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

tMeshData CAssetMgr::MakeCone(const float radius, const float height)
{
    tMeshData meshData;
    Vtx v;

    // Top
    v.vPos = Vec3(0.f, 0.f, 0.f);
    v.vUV = Vec2(0.5f, 0.f);
    v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    v.vNormal = Vec3(0.f, 0.f, -1.f);
    v.vTangent = Vec3(1.f, 0.f, 0.f);
    meshData.vertices.push_back(v);

    // Body
    UINT iSliceCount = 40; // 원뿔의 세로 분할 개수

    float fSliceAngle = XM_2PI / iSliceCount;

    float fUVXStep = 1.f / (float)iSliceCount;
    float fUVYStep = 1.f;

    for (UINT i = 0; i <= iSliceCount; ++i)
    {
        float theta = i * fSliceAngle;

        v.vPos = Vec3(radius * cosf(theta), radius * sinf(theta), height);
        v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
        v.vUV = Vec2(fUVXStep * i, fUVYStep);

        v.vNormal = Vec3(0.f, 0.f, 1.f);
        v.vTangent = Vec3(1.f, 0.f, 0.f);
        v.vNormal.Cross(v.vTangent, v.vBiTangent);
        v.vBiTangent.Normalize();
        meshData.vertices.push_back(v);

        // 인덱스
        if (i < iSliceCount)
        {
            meshData.indices.push_back(0);
            meshData.indices.push_back(i + 2);
            meshData.indices.push_back(i + 1);
        }
    }

    tMeshData circle2D = MakeCircle(radius, iSliceCount);
    {
        int offset = int(meshData.vertices.size());

        for (auto& vtx : circle2D.vertices)
        {
            vtx.vPos = Vector3::Transform(vtx.vPos, Matrix::CreateRotationX(XM_PI));
            vtx.vPos.z += height;
            meshData.vertices.push_back(vtx);
        }

        for (const auto& index : circle2D.indices)
        {
            meshData.indices.push_back(index + offset);
        }
    }

    return meshData;
}

tMeshData CAssetMgr::MakeWireCircle(const float radius, const int numPoints)
{
    tMeshData meshData;
    vector<Vtx>& vertices = meshData.vertices;
    vector<uint32_t>& indices = meshData.indices;

    const float dTheta = XM_2PI / float(numPoints);

    int offset = int(vertices.size());
    Vector3 start(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < numPoints; i++)
    {
        Vtx v;
        v.vPos = Vector3::Transform(start, Matrix::CreateRotationY(dTheta * float(i))) * radius;
        vertices.push_back(v);
        indices.push_back(i + offset);
        if (i != 0)
        {
            indices.push_back(i + offset);
        }
    }
    indices.push_back(offset);

    return meshData;
}

tMeshData CAssetMgr::MakeWireBox(const float scale)
{
    // 상자를 와이어 프레임으로 그리는 용도
    vector<Vector3> positions;

    // 앞면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);

    // 뒷면
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);

    tMeshData meshData;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vNormal = positions[i];
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

tMeshData CAssetMgr::MakeWireSphere(const float radius, const int numPoints)
{
    tMeshData meshData;
    vector<Vtx>& vertices = meshData.vertices;
    vector<uint32_t>& indices = meshData.indices;

    const float dTheta = XM_2PI / float(numPoints);

    // XY plane
    {
        int offset = int(vertices.size());
        Vector3 start(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < numPoints; i++)
        {
            Vtx v;
            v.vPos = Vector3::Transform(start, Matrix::CreateRotationZ(dTheta * float(i))) * radius;
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
            v.vPos = Vector3::Transform(start, Matrix::CreateRotationX(dTheta * float(i))) * radius;
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
            v.vPos = Vector3::Transform(start, Matrix::CreateRotationY(dTheta * float(i))) * radius;
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

tMeshData CAssetMgr::MakeWireCapsule2D(const float radius, const float halfHeight, const int numPoints)
{
    tMeshData meshData;
    vector<Vtx>& vertices = meshData.vertices;
    vector<uint32_t>& indices = meshData.indices;

    const float dTheta = XM_PI / float(numPoints);

    // Top
    {
        int offset = int(vertices.size());
        Vector3 start(-1.0f, 0.f, 0.0f);
        for (int i = 0; i < numPoints; i++)
        {
            Vtx v;
            v.vPos = Vector3::Transform(start, Matrix::CreateRotationZ(-dTheta * float(i))) * radius;
            v.vPos.y += halfHeight;
            vertices.push_back(v);
            indices.push_back(i + offset);
            if (i == 0)
                continue;
            indices.push_back(i + offset);
        }
    }

    // Bottom
    {
        int offset = int(vertices.size());
        Vector3 start(1.f, 0.f, 0.0f);
        for (int i = 0; i < numPoints; i++)
        {
            Vtx v;
            v.vPos = Vector3::Transform(start, Matrix::CreateRotationZ(-dTheta * float(i))) * radius;
            v.vPos.y -= halfHeight;
            vertices.push_back(v);
            indices.push_back(i + offset);
            indices.push_back(i + offset);
        }
    }
    indices.push_back(0);

    return meshData;
}

tMeshData CAssetMgr::MakeWireCapsule(const float radius, const float halfHeight, const int numPoints)
{
    tMeshData meshData;
    vector<Vtx>& vertices = meshData.vertices;
    vector<uint32_t>& indices = meshData.indices;

    tMeshData casule2D = MakeWireCapsule2D(radius, halfHeight, numPoints);

    // XY casule
    {
        for (const auto& vtx : casule2D.vertices)
            vertices.push_back(vtx);

        for (const auto& index : casule2D.indices)
            indices.push_back(index);
    }

    // YZ casule
    {
        int offset = int(vertices.size());

        for (auto& vtx : casule2D.vertices)
        {
            vtx.vPos = Vector3::Transform(vtx.vPos, Matrix::CreateRotationY(XM_PIDIV2));
            vertices.push_back(vtx);
        }

        for (const auto& index : casule2D.indices)
        {
            indices.push_back(index + offset);
        }
    }

    int circlenumPoints = numPoints;
    tMeshData circle2D = MakeWireCircle(radius, circlenumPoints * 2);
    // Top Circle
    {
        int offset = int(vertices.size());

        for (auto& vtx : circle2D.vertices)
        {
            vtx.vPos = Vector3::Transform(vtx.vPos, Matrix::CreateRotationY(XM_PIDIV2));
            vtx.vPos.y += halfHeight;
            vertices.push_back(vtx);
            vtx.vPos.y -= halfHeight; // 원상복귀
        }

        for (const auto& index : circle2D.indices)
        {
            indices.push_back(index + offset);
        }
    }

    // Bottom Circle
    {
        int offset = int(vertices.size());

        for (auto& vtx : circle2D.vertices)
        {
            vtx.vPos = Vector3::Transform(vtx.vPos, Matrix::CreateRotationY(XM_PIDIV2));
            vtx.vPos.y -= halfHeight;
            vertices.push_back(vtx);
            vtx.vPos.y += halfHeight; // 원상복귀
        }

        for (const auto& index : circle2D.indices)
        {
            indices.push_back(index + offset);
        }
    }

    return meshData;
}
