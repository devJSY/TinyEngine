#include "pch.h"
#include "CAssetMgr.h"

#include "CGraphicsShader.h"

void CAssetMgr::CreateDefaultGraphicsShader_Kirby()
{
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

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

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
    // Elfilis SkySphere Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\SkyBoxVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ElfilisSkyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddScalarParam(FLOAT_0, "Mask UV Scale");
        pShader->AddScalarParam(FLOAT_1, "Color UV Scale");
        pShader->AddScalarParam(FLOAT_2, "Emissive Power");
        pShader->AddScalarParam(VEC2_0, "Distortion Speed");
        pShader->AddScalarParam(VEC2_1, "Distortion Scale");

        pShader->AddTexParam(TEX_0, "Color Masking Texture");
        pShader->AddTexParam(TEX_1, "Noise Texture");
        pShader->AddTexParam(TEX_2, "ColorMap Texture");
        pShader->AddTexParam(TEX_4, "Color Nosie Texture");
        pShader->AddTexParam(TEX_3, "Shadow Texture");
        pShader->AddTexParam(TEX_5, "Emissive Masking Texture");

        pShader->SetName(L"ElfilisSkySphereShader");
        AddAsset(L"ElfilisSkySphereShader", pShader);
    }

    // =================================
    // Dash Effect Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\DashEffectMaskingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddTexParam(TEX_0, "Masking Texture1");
        pShader->AddTexParam(TEX_1, "Gradation Texture");

        pShader->AddTexParam(TEX_5, "Normal Wind Masking Texture1");

        pShader->AddScalarParam(INT_0, "Invert NormalMapping");
        pShader->AddScalarParam(VEC4_0, "Dash Color", 0.1f);

        pShader->AddScalarParam(VEC2_0, "Masking Texture1 Scale UVOffset", 0.1f);
        pShader->AddScalarParam(VEC2_1, "Masking Texture2 Scael UVOffset", 0.1f);
        pShader->AddScalarParam(VEC2_2, "BackGround Texture Offset", 0.1f);

        pShader->AddScalarParam(FLOAT_0, "Progress Value", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "ThreshHold", 0.1f);
        pShader->AddScalarParam(FLOAT_2, "BackGround Alpha", 0.1f);
        pShader->AddScalarParam(FLOAT_3, "Pow Value", 0.1f);

        pShader->SetName(L"DashEffectMaskingShader");
        AddAsset(L"DashEffectMaskingShader", pShader);
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

        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");

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

        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");

        pShader->SetName(L"NoddyShader");
        AddAsset(L"NoddyShader", pShader);
    }

    // =================================
    // HotHead Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\HotHeadPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Eye Base Texture");
        pShader->AddTexParam(TEX_1, "Skin Base Texture");
        pShader->AddTexParam(TEX_2, "Skin Normal Texture");
        pShader->AddTexParam(TEX_3, "Skin MRA Texture");

        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");

        pShader->SetName(L"HotHeadShader");
        AddAsset(L"HotHeadShader", pShader);
    }

    // =================================
    // SirKibble Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\SirKibblePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Skin Base Texture");
        pShader->AddTexParam(TEX_1, "Skin Normal Texture");
        pShader->AddTexParam(TEX_2, "Skin MRA Texture");

        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");

        pShader->SetName(L"SirKibbleShader");
        AddAsset(L"SirKibbleShader", pShader);
    }

    // =================================
    // GhostGordo Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\GhostGordoPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddTexParam(TEX_0, "Skin Base Texture");
        pShader->AddTexParam(TEX_1, "Skin Normal Texture");
        pShader->AddTexParam(TEX_2, "Skin MRA Texture");
        pShader->AddTexParam(TEX_3, "Skin Emissive Texture");

        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");

        pShader->SetName(L"GhostGordoShader");
        AddAsset(L"GhostGordoShader", pShader);
    }

    // =================================
    // Elfilis Body Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ElfilisBodyPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        string TeleprotDesc = "[Teleport Info]\n- (x) Flag(false, Down, Up)\n- (y) Teleport WorldY\n- (z) Radius";
        pShader->AddScalarParam(VEC4_3, TeleprotDesc);

        pShader->AddScalarParam(INT_0, "Invert NormalMapY");
        pShader->AddScalarParam(FLOAT_0, "HeightMapping Scale", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");

        pShader->AddTexParam(TEX_0, "Albedo0 Texture");
        pShader->AddTexParam(TEX_1, "Albedo1 Texture");
        pShader->AddTexParam(TEX_2, "Albedo2 Texture");
        pShader->AddTexParam(TEX_3, "Albedo3 Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Height Texture");
        pShader->AddTexParam(TEX_7, "Emissive Texture");

        pShader->SetName(L"ElfilisBodyShader");
        AddAsset(L"ElfilisBodyShader", pShader);
    }

    // =================================
    // Elfilis Tail Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\MaskingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        string TeleprotDesc = "[Teleport Info]\n- (x) Flag(false, Down, Up)\n- (y) Teleport WorldY\n- (z) Radius";
        pShader->AddScalarParam(VEC4_3, TeleprotDesc);

        pShader->AddScalarParam(INT_0, "Invert NormalMap Y");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(VEC4_1, "Additional Color");
        pShader->AddScalarParam(INT_1, "Invert Masking Alpha 0");
        pShader->AddScalarParam(INT_2, "Invert Masking Alpha 1");

        pShader->AddTexParam(TEX_0, "Masking Alpha0 Texture");
        pShader->AddTexParam(TEX_1, "Masking Alpha1 Texture");
        pShader->AddTexParam(TEX_2, "Masking Color Texture");
        pShader->AddTexParam(TEX_3, "Color Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Emissive Texture");

        pShader->SetName(L"ElfilisTailShader");
        AddAsset(L"ElfilisTailShader", pShader);
    }

    // =================================
    // Transparent Elfilis Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\TransparentElfilisPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(INT_0, "Invert NormalMap Y");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(FLOAT_2, "Mtrl Alpha");
        pShader->AddScalarParam(FLOAT_3, "Alpha Threshold");

        pShader->AddTexParam(TEX_0, "Albedo Texture");
        pShader->AddTexParam(TEX_1, "Noise Alpha Texture");
        pShader->AddTexParam(TEX_4, "Noise Color Texture");
        pShader->AddTexParam(TEX_2, "Normal Texture");
        pShader->AddTexParam(TEX_3, "Emissive Texture");

        pShader->SetName(L"TransparentElfilisShader");
        AddAsset(L"TransparentElfilisShader", pShader);
    }

    // =================================
    // Elfilis Laser Outer Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ElfilisLaserOuterPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        string RenderMaskFlagDesc = "[Render Mask Flag]\n- (0) NONE, (1) LocalZ, (2) WorldY";
        string RenderMaskInfoDesc = "[Render Mask Info]\n- (x) Use From Pos : (0) NONE\n- (y) Use To Pos : (0) NONE\n- (z) From Pos\n- (w) To Pos";
        pShader->AddScalarParam(SCALAR_PARAM::INT_0, RenderMaskFlagDesc);
        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, RenderMaskInfoDesc);

        pShader->AddTexParam(TEX_0, "Nosie Black Texture");
        pShader->AddTexParam(TEX_1, "Noise Alpha Texture");
        pShader->AddTexParam(TEX_2, "Emissive Texture");

        pShader->SetName(L"ElfilisLaserOuterShader");
        AddAsset(L"ElfilisLaserOuterShader", pShader);
    }

    // =================================
    // Elfilis Laser Inner Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\ElfilisLaserInnerPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        string RenderMaskFlagDesc = "[Render Mask Flag]\n- (0) NONE, (1) LocalZ, (2) WorldY";
        string RenderMaskInfoDesc = "[Render Mask Info]\n- (x) Use From Pos : (0) NONE\n- (y) Use To Pos : (0) NONE\n- (z) From Pos\n- (w) To Pos";
        pShader->AddScalarParam(SCALAR_PARAM::INT_0, RenderMaskFlagDesc);
        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, RenderMaskInfoDesc);

        pShader->AddTexParam(TEX_0, "Emissive Noise Texture");

        pShader->SetName(L"ElfilisLaserInnerShader");
        AddAsset(L"ElfilisLaserInnerShader", pShader);
    }

    // =================================
    // UIDefaultShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIDefaultPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(FLOAT_0, "AlphaDiscard Threshold", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Glow Point", 0.1f);

        pShader->AddScalarParam(INT_0, "UIANIM1", 0.1f);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"KirbyUIDefaulthader");
        AddAsset(L"KirbyUIDefaultShader", pShader);
    }

    // =================================
    // UIDefaultNoTestShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIDefaultPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(FLOAT_0, "AlphaDiscard Threshold", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Glow Point", 0.1f);

        pShader->AddScalarParam(INT_0, "UIANIM1", 0.1f);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"UIDefaultNoTestShader");
        AddAsset(L"UIDefaultNoTestShader", pShader);
    }

    // =================================
    // UIDefaultBluredMaskShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyUIDefaultBluredPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddTexParam(TEX_0, "Blured Texture");
        pShader->AddTexParam(TEX_1, "PostProcess Texture");

        pShader->AddScalarParam(FLOAT_0, "Blur Amount", 0.0001f);

        pShader->SetName(L"UIDefaultBluredMaskShader");
        AddAsset(L"UIDefaultBluredMaskShader", pShader);
    }

    // =================================
    // UICircleProgressShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UICircleProgressPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(FLOAT_0, "AlphaDiscard Threshold", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Test Radian", 0.1f);

        pShader->AddScalarParam(VEC4_0, "Static Color", 0.1f);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"UICircleProgressShader");
        AddAsset(L"UICircleProgressShader", pShader);
    }

    // =================================
    // Kirby UI StartScene Button Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIStartSceneButtonPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(FLOAT_0, "AlphaDiscard Threshold", 0.1f);
        pShader->AddScalarParam(INT_0, "USE Tex Color", 1.f);

        pShader->AddScalarParam(VEC4_0, "Static Color", 0.1f);

        pShader->AddTexParam(TEX_0, "Shaping Texture");
        pShader->AddTexParam(TEX_1, "Color Texture");

        pShader->SetName(L"KirbyUIStartSceneButtonShader");
        AddAsset(L"KirbyUIStartSceneButtonShader", pShader);
    }

    // =================================
    // Kirby UI Lobby Scene BackGround Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIRobbyBackGroundPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddScalarParam(FLOAT_0, "AlphaDiscard Threshold", 0.1f);

        pShader->AddScalarParam(VEC4_0, "BackGround Color", 0.1f);

        pShader->AddTexParam(TEX_0, "Pattern Texture1");
        pShader->AddTexParam(TEX_1, "Pattern Texture2");

        pShader->SetName(L"KirbyUIRobbySceneBGShader");
        AddAsset(L"KirbyUIRobbySceneBGShader", pShader);
    }

    // =================================
    // Kirby UI Masking Anim 2D Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIMaskingAnimPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Masking Texture");

        pShader->SetName(L"KirbyUIMaskingAnim2DShader");
        AddAsset(L"KirbyUIMaskingAnim2DShader", pShader);
    }

    // =================================
    // UIKirbyHPShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIHPShaderPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"KirbyUIHPShader");
        AddAsset(L"KirbyUIHPShader", pShader);
    }

    // =================================
    // KirbyStandardUIShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"KirbyStandardUIShader");
        AddAsset(L"KirbyStandardUIShader", pShader);
    }

    // =================================
    // KirbyFontUIShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\UIKirbyFontPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");
        pShader->AddScalarParam(FLOAT_0, "RedThreshHold");
        pShader->AddScalarParam(VEC4_0, "Static Color");

        pShader->SetName(L"KirbyUIFontShader");
        AddAsset(L"KirbyUIFontShader", pShader);
    }

    // =================================
    // KirbyClearLineUIShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\UIKirbyClearLineShader.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

        pShader->AddTexParam(TEX_0, "Texture");
        pShader->AddScalarParam(FLOAT_0, "RedThreshHold");

        pShader->AddScalarParam(VEC4_0, "Static Color");
        pShader->AddScalarParam(VEC4_1, "Static Color2");

        pShader->SetName(L"KirbyClearLineUIShader");
        AddAsset(L"KirbyClearLineUIShader", pShader);
    }

    // =================================
    // 2DEffectMaskingShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\EffectMaskingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS_EQUAL);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"2DMaskingEffectShader");
        AddAsset(L"2DMaskingEffectShader", pShader);
    }

    // =================================
    // 2DEffectMaskingNoTestShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\EffectMaskingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->SetName(L"2DEffectMaskingNoTestShader");
        AddAsset(L"2DEffectMaskingNoTestShader", pShader);
    }

    // =================================
    // UIKirbyHPOutLineShader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UIDefaultVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\UIHPOutLinePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

        pShader->AddTexParam(TEX_0, "Texture");
        pShader->AddScalarParam(FLOAT_0, "ALPHATHRESHHOLD");

        pShader->SetName(L"KirbyUIHPOutLineShader");
        AddAsset(L"KirbyUIHPOutLineShader", pShader);
    }

    // =================================
    // Effect Masking Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\MaskingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(INT_0, "Invert NormalMap Y");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color");
        pShader->AddScalarParam(VEC4_1, "Additional Color");
        pShader->AddScalarParam(INT_1, "Invert Masking Alpha 0");
        pShader->AddScalarParam(INT_2, "Invert Masking Alpha 1");

        pShader->AddTexParam(TEX_0, "Masking Alpha0 Texture");
        pShader->AddTexParam(TEX_1, "Masking Alpha1 Texture");
        pShader->AddTexParam(TEX_2, "Masking Color Texture");
        pShader->AddTexParam(TEX_3, "Color Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Emissive Texture");

        pShader->SetName(L"EffectMaskingShader");
        AddAsset(L"EffectMaskingShader", pShader);
    }

    // =================================
    // Effect DimensionGate
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\DimensionGatePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(INT_0, "Invert NormalMap Y");
        pShader->AddScalarParam(FLOAT_2, "Color Coefficient");
        pShader->AddScalarParam(FLOAT_1, "Distortion Speed");
        pShader->AddScalarParam(FLOAT_3, "Distortion Scale");
        pShader->AddScalarParam(VEC4_0, "Noise Color");

        pShader->AddTexParam(TEX_0, "Masking Alpha Texture");
        pShader->AddTexParam(TEX_1, "Color Texture");
        pShader->AddTexParam(TEX_2, "Base Color Noise Texture");
        pShader->AddTexParam(TEX_3, "Distortion Noise Texture");

        pShader->SetName(L"EffectDimensionGateShader");
        AddAsset(L"EffectDimensionGateShader", pShader);
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

        pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Invert Normal Y");

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
    // Glass BackGround Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\GlassPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT_BACKGROUND);

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

        pShader->SetName(L"GlassBackGroundShader");
        AddAsset(L"GlassBackGroundShader", pShader);
    }

    // =================================
    // Rim Only Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\RimOnlyPs.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(FLOAT_2, "Change Duration");
        pShader->AddScalarParam(FLOAT_1, "Rim Power");
        pShader->AddScalarParam(VEC4_0, "Rim Color 1");
        pShader->AddScalarParam(VEC4_1, "Rim Color 2");
        pShader->AddScalarParam(FLOAT_3, "Background Alpha");

        pShader->SetName(L"RimOnlyShader");
        AddAsset(L"RimOnlyShader", pShader);
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

        pShader->AddScalarParam(INT_0, "Normal MRA Apply UV Scale");

        pShader->AddScalarParam(FLOAT_0, "Albedo0 UV Scale", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Albedo1 UV Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Albedo0 Texture");
        pShader->AddTexParam(TEX_1, "MRA0 Texture");
        pShader->AddTexParam(TEX_2, "Normal0 Texture");

        pShader->AddTexParam(TEX_3, "Albedo1 Texture");
        pShader->AddTexParam(TEX_4, "MRA1 Texture");
        pShader->AddTexParam(TEX_5, "Normal1 Texture");

        pShader->SetName(L"KirbyMapShader");
        AddAsset(L"KirbyMapShader", pShader);
    }

    // =================================
    // FunHouse Map Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\FunHouseMapPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

        pShader->AddScalarParam(INT_0, "Normal MRA Apply UV Scale");

        pShader->AddScalarParam(FLOAT_0, "Albedo0 UV Scale", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Albedo1 UV Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Albedo0 Texture");
        pShader->AddTexParam(TEX_1, "MRA0 Texture");
        pShader->AddTexParam(TEX_2, "Normal0 Texture");
        pShader->AddTexParam(TEX_3, "Emissive0 Texture");

        pShader->AddTexParam(TEX_4, "Albedo1 Texture");
        pShader->AddTexParam(TEX_5, "MRA1 Texture");
        pShader->AddTexParam(TEX_6, "Normal1 Texture");
        pShader->AddTexParam(TEX_7, "Emissive1 Texture");

        pShader->SetName(L"FunHouseMapShader");
        AddAsset(L"FunHouseMapShader", pShader);
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

        pShader->AddScalarParam(INT_0, "Use Alpha");
        pShader->AddScalarParam(FLOAT_2, "Alpha", 0.1f);
        pShader->AddScalarParam(FLOAT_0, "UV Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Albedo Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Height Texture");
        pShader->AddTexParam(TEX_7, "Emissive Texture");

        pShader->SetName(L"TransparentShader");
        AddAsset(L"TransparentShader", pShader);
    }

    // =================================
    // Transparent BackGround Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\TransparentPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT_BACKGROUND);

        pShader->AddScalarParam(FLOAT_0, "UV Scale", 0.1f);

        pShader->AddTexParam(TEX_0, "Albedo Texture");
        pShader->AddTexParam(TEX_4, "MRA Texture"); // Metallic, Roughness, Ambient Occlusion
        pShader->AddTexParam(TEX_5, "Normal Texture");
        pShader->AddTexParam(TEX_6, "Height Texture");
        pShader->AddTexParam(TEX_7, "Emissive Texture");

        pShader->SetName(L"TransparentBackGroundShader");
        AddAsset(L"TransparentBackGroundShader", pShader);
    }

    // =================================
    // Kirby Fade Effect Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyFadeEffectPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddTexParam(TEX_0, "Mask Texture");
        pShader->AddScalarParam(FLOAT_0, "Weight", 1e-3f);
        pShader->AddScalarParam(FLOAT_1, "Rotate Angle", 1e-3f);
        pShader->AddScalarParam(VEC4_0, "BackGroundColor");
        pShader->AddScalarParam(VEC4_1, "Target NDC Pos");

        pShader->SetName(L"KirbyFadeEffectShader");
        AddAsset(L"KirbyFadeEffectShader", pShader);
    }

    // =================================
    // Kirby DimensionalFade Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyDimensionalFadePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->SetName(L"KirbyDimensionalFadeShader");
        AddAsset(L"KirbyDimensionalFadeShader", pShader);
    }

    // =================================
    // Kirby Vignette Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\postprocessVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyVignettePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(FLOAT_0, "Red Color", 0.01f);
        pShader->AddScalarParam(FLOAT_1, "Green Color", 0.01f);
        pShader->AddScalarParam(FLOAT_2, "Blue Color", 0.01f);

        pShader->AddScalarParam(VEC2_0, "Inner Value", 0.01f);

        pShader->SetName(L"KirbyVignetteShader");
        AddAsset(L"KirbyVignetteShader", pShader);
    }

    // =================================
    // Fire Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\FirePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddTexParam(TEX_0, "Masking Texture");
        pShader->AddScalarParam(FLOAT_0, "Lerp Ratio", 0.1f);
        pShader->AddScalarParam(VEC4_0, "Init Fire Color", 0.1f);
        pShader->AddScalarParam(VEC4_1, "End Fire Color", 0.1f);

        pShader->SetName(L"FireShader");
        AddAsset(L"FireShader", pShader);
    }

    // =================================
    // Wing Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\KirbyDragonWingPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddTexParam(TEX_0, "ScrollGrd Texture");
        pShader->AddTexParam(TEX_1, "Masking Texture");
        pShader->AddScalarParam(FLOAT_0, "Alpha Weight", 0.1f);
        pShader->AddScalarParam(FLOAT_1, "Color Weight", 0.1f);
        pShader->AddScalarParam(VEC4_0, "Init Fire Color", 0.1f);
        pShader->AddScalarParam(VEC4_1, "End Fire Color", 0.1f);

        pShader->SetName(L"KirbyDragonWingPS");
        AddAsset(L"KirbyDragonWingPS", pShader);
    }

    // =================================
    // Tornado Effect Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\TornadoEffectPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddTexParam(TEX_0, "Masking Texture");

        pShader->AddScalarParam(FLOAT_0, "Alpha");
        pShader->AddScalarParam(VEC4_0, "Tornado Color");

        pShader->SetName(L"TornadoEffectShader");
        AddAsset(L"TornadoEffectShader", pShader);
    }

    // =================================
    // Effect Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\EffectPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddTexParam(TEX_0, "Texture");

        pShader->AddScalarParam(FLOAT_0, "Speed");
        pShader->AddScalarParam(VEC4_0, "Color 1");
        pShader->AddScalarParam(VEC4_1, "Color 2");

        pShader->SetName(L"EffectShader");
        AddAsset(L"EffectShader", pShader);
    }

    // =================================
    // Barricade Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BarricadePS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT_BACKGROUND);

        pShader->AddTexParam(TEX_0, "Texture 0");
        pShader->AddTexParam(TEX_1, "Texture 1");

        pShader->AddScalarParam(FLOAT_0, "Speed", 1e-3f);
        pShader->AddScalarParam(FLOAT_1, "Alpha", 1e-3f);
        pShader->AddScalarParam(VEC2_0, "UV Scale", 1e-3f);
        pShader->AddScalarParam(VEC4_0, "Color", 1e-3f);

        pShader->SetName(L"BarricadeShader");
        AddAsset(L"BarricadeShader", pShader);
    }

    // =================================
    // ParticleRender ColorMap
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\particle_render.fx", "VS_ParticleRender");
        pShader->CreateGeometryShader(L"shader\\particle_render.fx", "GS_ParticleRender");
        pShader->CreatePixelShader(L"shader\\particle_render.fx", "PS_ParticleRender_ColorMap");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "White Mapping Color");
        pShader->AddTexParam(TEX_0, "Mapping Texture");

        pShader->SetName(L"ParticleRenderColorMapShader");
        AddAsset(L"ParticleRenderColorMapShader", pShader);
    }

    // =================================
    // Effect Quad Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\EffectQuadPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트는 진행, 깊이는 기록 X
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "Alpha");
        pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Use Change Color");
        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Change Color");
        pShader->AddTexParam(TEX_0, "Quad Texture");

        pShader->SetName(L"EffectQuadShader");
        AddAsset(L"EffectQuadShader", pShader);
    }

    // =================================
    // Effect Texture Distortion Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\TextureDistortionPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(SCALAR_PARAM::VEC2_0, "Distortion Speed");
        pShader->AddScalarParam(SCALAR_PARAM::VEC2_1, "Distortion Scale");
        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Use VtxColor Scale");
        pShader->AddTexParam(TEX_0, "Mask Texture");
        pShader->AddTexParam(TEX_1, "Noise Texture");

        pShader->SetName(L"EffectTextureDistortionShader");
        AddAsset(L"EffectTextureDistortionShader", pShader);
    }

    // =================================
    // Texture Distortion Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\TextureDistortionPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->AddScalarParam(SCALAR_PARAM::VEC2_0, "Distortion Speed");
        pShader->AddScalarParam(SCALAR_PARAM::VEC2_1, "Distortion Scale");
        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Use VtxColor Scale");
        pShader->AddTexParam(TEX_0, "Mask Texture");
        pShader->AddTexParam(TEX_1, "Noise Texture");

        pShader->SetName(L"TextureDistortionRenderShader");
        AddAsset(L"TextureDistortionRenderShader", pShader);
    }

    // =================================
    // Mesh Distortion Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\MeshDistortionPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

        pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Use VtxColor Scale");
        pShader->AddTexParam(TEX_0, "Noise Texture");
        pShader->AddTexParam(TEX_1, "Masking Texture");

        pShader->SetName(L"MeshDistortionShader");
        AddAsset(L"MeshDistortionShader", pShader);
    }

    // =================================
    // Vacuum Effect Shader
    // =================================
    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\VacuumEffectPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);


        pShader->AddScalarParam(FLOAT_0, "Gradation Alpha");

        pShader->AddTexParam(TEX_0, "Color Texture");
        pShader->AddTexParam(TEX_1, "Alpha Texture");
        pShader->AddTexParam(TEX_2, "Gradation Texture");



        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->SetName(L"VacuumEffectShader");
        AddAsset(L"VacuumEffectShader", pShader);
    }

    {
        Ptr<CGraphicsShader> pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\UnrealPBRVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\VacuumEffectPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_FRONT);
        pShader->SetDSType(DS_TYPE::NO_WRITE);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        pShader->AddScalarParam(FLOAT_0, "Gradation Alpha");

        pShader->AddTexParam(TEX_0, "Color Texture");
        pShader->AddTexParam(TEX_1, "Alpha Texture");
        pShader->AddTexParam(TEX_2, "Gradation Texture");

        pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

        pShader->SetName(L"VacuumInsideEffectShader");
        AddAsset(L"VacuumInsideEffectShader", pShader);
    }
}