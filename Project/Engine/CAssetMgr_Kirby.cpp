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

        pShader->AddScalarParam(INT_0, "Body");
        pShader->AddScalarParam(INT_1, "Mouth");
        pShader->AddTexParam(TEX_0, "Eye Base Texture");
        pShader->AddTexParam(TEX_1, "Eye Mask Texture");
        pShader->AddTexParam(TEX_2, "Eye Normal Texture");
        pShader->AddTexParam(TEX_3, "Skin Texture");
        pShader->AddTexParam(TEX_4, "Mouth Texture");

        pShader->SetName(L"KirbyBodyShader");
        AddAsset(L"KirbyBodyShader", pShader);
    }
}