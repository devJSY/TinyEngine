#include "pch.h"
#include "CAssetMgr.h"
#include "CGraphicsShader.h"

void CAssetMgr::CreateDefaultGraphicsShader_Kirby()
{
    // =================================
    // Unreal PBR Deferred Shader
    // =================================
    //{
    //    Ptr<CGraphicsShader> pShader = new CGraphicsShader;
    //    pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
    //    pShader->CreatePixelShader(L"shader\\UnrealPBRDeferredPS.hlsl", "main");

    //    pShader->SetRSType(RS_TYPE::CULL_BACK);
    //    pShader->SetDSType(DS_TYPE::LESS);
    //    pShader->SetBSType(BS_TYPE::DEFAULT);

    //    pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

    //    pShader->AddScalarParam(INT_0, "Invert NormalMapY");
    //    pShader->AddScalarParam(FLOAT_0, "HeightMapping Scale", 0.1f);

    //    pShader->AddTexParam(TEX_0, "Albedo0 Texture");
    //    pShader->AddTexParam(TEX_1, "Albedo1 Texture");
    //    pShader->AddTexParam(TEX_2, "Albedo2 Texture");
    //    pShader->AddTexParam(TEX_3, "Albedo3 Texture");
    //    pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
    //    pShader->AddTexParam(TEX_5, "Normal Texture");
    //    pShader->AddTexParam(TEX_6, "Height Texture");
    //    pShader->AddTexParam(TEX_7, "Emissive Texture");

    //    pShader->SetName(L"UnrealPBRDeferredShader");
    //    AddAsset(L"UnrealPBRDeferredShader", pShader);
    //}

    // =================================
    // Kirby Body Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyBodyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Eye Base Texture");
        pShader->AddTexParam(TEX_1, "Eye Mask Texture");
        pShader->AddTexParam(TEX_2, "Eye Normal Texture");
        pShader->AddTexParam(TEX_3, "Skin Texture");
        pShader->AddTexParam(TEX_4, "Mouth Texture");

        pShader->SetName(L"KirbyBodyShader");
        AddAsset(L"KirbyBodyShader", pShader);
    }

    // =================================
    // Kirby Lightbulb Body Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyLightbulbBodyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Eye Base Texture");
        pShader->AddTexParam(TEX_1, "Eye Mask Texture");
        pShader->AddTexParam(TEX_2, "Eye Normal Texture");
        pShader->AddTexParam(TEX_3, "Skin Texture");
        pShader->AddTexParam(TEX_4, "Mouth Texture");

        pShader->SetName(L"KirbyLibhtbulbBodyShader");
        AddAsset(L"KirbyLightbulbBodyShader", pShader);
    }

    // =================================
    // Kirby SkySphere Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\SkyBoxVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbySkySpherePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEX_0, "SkySphere Texture");

        pShader->SetName(L"KirbySkySphereShader");
        AddAsset(L"KirbySkySphereShader", pShader);
    }

    // =================================
    // Normal Enemy Body Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\NormalEnemyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Eye Base Texture");
        pShader->AddTexParam(TEX_1, "Skin Base Texture");
        pShader->AddTexParam(TEX_2, "Skin Normal Texture");
        pShader->AddTexParam(TEX_3, "Skin MRA Texture");

        pShader->SetName(L"NormalEnemyBodyShader");
        AddAsset(L"NormalEnemyBodyShader", pShader);
    }

    // =================================
    // Noddy Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\NoddyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Eye Texture");
        pShader->AddTexParam(TEX_1, "Skin Texture");
        pShader->AddTexParam(TEX_2, "MRA Texture");
        pShader->AddTexParam(TEX_3, "Normal Texture");

        pShader->SetName(L"NoddyShader");
        AddAsset(L"NoddyShader", pShader);
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

        pShader->SetName(L"KirbyUIHPShader");
        AddAsset(L"KirbyUIHPShader", pShader);
    }

    // =================================
    // Kirby Sword Magma Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbySwordMagmaPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Base Texture");
        pShader->AddTexParam(TEX_1, "MRA Texture");
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_2, "Magma Texture");

        pShader->SetName(L"KirbySwordMagmaShader");
        AddAsset(L"KirbySwordMagmaShader", pShader);
    }

    // =================================
    // Kirby Butterfly Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyButterflyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Wing(Down) Texture");
        pShader->AddTexParam(TEX_1, "Wing(Up) Texture");
        pShader->AddTexParam(TEX_2, "Magma Texture");

        pShader->SetName(L"KirbyButterflyShader");
        AddAsset(L"KirbyButterflyShader", pShader);
    }

    // =================================
    // Glass Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\GlassPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_2, "Alpha");

        pShader->AddTexParam(TEX_0, "Albedo0 Texture");
        pShader->AddTexParam(TEX_1, "Albedo1 Texture");
        pShader->AddTexParam(TEX_2, "Albedo2 Texture");
        pShader->AddTexParam(TEX_3, "Albedo3 Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Height Texture");
        pShader->AddTexParam(TEX_7, "Emissive Texture");

        pShader->SetName(L"GlassShader");
        AddAsset(L"GlassShader", pShader);
    }

    // =================================
    // Kirby Map Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyMapPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddScalarParam(FLOAT_0, "UV Scale", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Mask UV Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Albedo0 Texture");
        pShader->AddTexParam(TEX_1, "MRA0 Texture");
        pShader->AddTexParam(TEX_2, "Normal0 Texture");

        pShader->AddTexParam(TEX_3, "Albedo1 Texture");
        pShader->AddTexParam(TEX_4, "MRA1 Texture");
        pShader->AddTexParam(TEX_5, "Normal1 Texture");
        
        pShader->AddTexParam(TEX_6, "Mask Texture");

        pShader->SetName(L"KirbyMapShader");
        AddAsset(L"KirbyMapShader", pShader);
    }

    // =================================
    // Transparent Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\TransparentPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(FLOAT_0, "UV Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Albedo Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Height Texture");
        pShader->AddTexParam(TEX_7, "Emissive Texture");

        pShader->SetName(L"TransparentShader");
        AddAsset(L"TransparentShader", pShader);
    }
}