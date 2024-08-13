#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

// Texture 0 Set
#define Albedo0Tex g_tex_0
#define MRA0Tex g_tex_1
#define Noraml0Tex g_tex_2
#define Emissive0Tex g_tex_3

// Texture 1 Set
#define Albedo1Tex g_tex_4
#define MRA1Tex g_tex_5
#define Noraml1Tex g_tex_6
#define Emissive1Tex g_tex_7

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define Albedo0UVScale g_float_0
#define Albedo1UVScale g_float_1

#define UVApply g_int_0

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vTangent : SV_Target3;
    float4 vBitangent : SV_Target4;
    float4 vEmissive : SV_Target5;
    float4 vMRA : SV_Target6;
};

PS_OUT main(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0.f;
    PS_OUT output0 = (PS_OUT) 0.f;
    PS_OUT output1 = (PS_OUT) 0.f;
    
    float4 albedo0 = g_btex_0 ? Albedo0Tex.Sample(g_LinearWrapSampler, Albedo0UVScale > 0.f ? input.vUV0 * (1.f / Albedo0UVScale) : input.vUV0) : (float4) 0.f;
    float4 albedo1 = g_btex_4 ? Albedo1Tex.Sample(g_LinearWrapSampler, Albedo1UVScale > 0.f ? input.vUV0 * (1.f / Albedo1UVScale) : input.vUV0) : (float4) 0.f;
    
    float2 vUV0 = (UVApply && Albedo0UVScale > 0.f) ? input.vUV0 * (1.f / Albedo0UVScale) : input.vUV0;
    float2 vUV1 = (UVApply && Albedo1UVScale > 0.f) ? input.vUV0 * (1.f / Albedo1UVScale) : input.vUV0;
    
    albedo0.rgb *= albedo0.a;
    albedo1.rgb *= albedo1.a;
    
    bool bDirty = false;
    
    // Texture 0 Set 
    if (albedo0.a >= 0.1f)
    {
        bDirty = true;
        
        output0.vColor = albedo0;
        output0.vPosition = float4(input.vPosWorld, 1.f);
        output0.vNormal = float4(g_btex_2 ? NormalMapping(input, Noraml0Tex, vUV0, g_LinearWrapSampler, true) : normalize(input.vNormalWorld), 1.f);
        output0.vTangent = float4(input.vTangentWorld, 1.f);
        output0.vBitangent = float4(input.vBitangentWorld, 1.f);
        output0.vEmissive = g_btex_3 ? Emissive0Tex.Sample(g_LinearWrapSampler, vUV0) : MtrlEmission;
        
        float4 MRAColor = g_btex_1 ? MRA0Tex.Sample(g_LinearWrapSampler, vUV0) : (float4) 0.f;
    
        float metallic = g_btex_1 ? MRAColor.x : MtrlMetallic;
        float roughness = g_btex_1 ? MRAColor.y : MtrlRoughness;
        float ao = g_btex_1 ? MRAColor.z : 1.f;
        if (ao >= 1.f)
        {
            ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
        }
        
        output0.vMRA = float4(metallic, roughness, ao, 1.f);
    }
    
    // Texture 1 Set 
    if (albedo1.a >= 0.1f)
    {
        bDirty = true;
        
        output1.vColor = albedo1;
        output1.vPosition = float4(input.vPosWorld, 1.f);
        output1.vNormal = float4(g_btex_6 ? NormalMapping(input, Noraml1Tex, vUV1, g_LinearWrapSampler, true) : normalize(input.vNormalWorld), 1.f);
        output1.vTangent = float4(input.vTangentWorld, 1.f);
        output1.vBitangent = float4(input.vBitangentWorld, 1.f);
        output1.vEmissive = g_btex_7 ? Emissive1Tex.Sample(g_LinearWrapSampler, vUV1) : MtrlEmission;
        
        float4 MRAColor = g_btex_5 ? MRA1Tex.Sample(g_LinearWrapSampler, vUV1) : (float4) 0.f;
    
        float metallic = g_btex_5 ? MRAColor.x : MtrlMetallic;
        float roughness = g_btex_5 ? MRAColor.y : MtrlRoughness;
        float ao = g_btex_5 ? MRAColor.z : 1.f;
        if (ao >= 1.f)
        {
            ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
        }
        
        output1.vMRA = float4(metallic, roughness, ao, 1.f);
    }
    
    // 바인딩되어있는 텍스춰가 존재하는 경우
    if (bDirty)
    {
        // 버텍스 컬러값을 가중치로 사용
        float MaskAlpha = 3.f - dot(input.vColor.rgb, float3(1.f, 1.f, 1.f));
        MaskAlpha = 1.f - MaskAlpha;
        
        output.vColor = (output0.vColor * MaskAlpha) + (output1.vColor * (1.f - MaskAlpha));
        output.vPosition = float4(input.vPosWorld, 1.f);
        output.vNormal = (output0.vNormal * MaskAlpha) + (output1.vNormal * (1.f - MaskAlpha));
        output.vTangent = (output0.vTangent * MaskAlpha) + (output1.vTangent * (1.f - MaskAlpha));
        output.vBitangent = (output0.vBitangent * MaskAlpha) + (output1.vBitangent * (1.f - MaskAlpha));
        output.vEmissive = (output0.vEmissive * MaskAlpha) + (output1.vEmissive * (1.f - MaskAlpha));
        output.vMRA = (output0.vMRA * MaskAlpha) + (output1.vMRA * (1.f - MaskAlpha));
    }
    else
    {
        output.vColor = input.vColor;
        output.vPosition = float4(input.vPosWorld, 1.f);
        output.vNormal = float4(input.vNormalWorld, 1.f);
        output.vTangent = float4(input.vTangentWorld, 1.f);
        output.vBitangent = float4(input.vBitangentWorld, 1.f);
        output.vEmissive = MtrlEmission;
        output.vMRA = float4(MtrlMetallic, MtrlRoughness, SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r, 1.f);
    }
    
    return output;
}