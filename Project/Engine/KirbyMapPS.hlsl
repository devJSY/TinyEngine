#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex g_tex_0
#define MRA0Tex g_tex_1
#define Noraml0Tex g_tex_2

#define Albedo1Tex g_tex_3
#define MRA1Tex g_tex_4
#define Noraml1Tex g_tex_5

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

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

    float4 albedo0 = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 albedo1 = g_btex_1 ? Albedo1Tex.Sample(g_LinearClampSampler, input.vUV1) : (float4) 0.f;
    
    albedo0.rgb *= albedo0.a;
    albedo1.rgb *= albedo1.a;
    
    if (albedo0.a >= 0.1f)
    {
        output.vColor = albedo0;
        output.vPosition = float4(input.vPosWorld, 1.f);
        output.vNormal = float4(g_btex_1 ? NormalMapping(input, Noraml0Tex, input.vUV0, g_LinearWrapSampler) : normalize(input.vNormalWorld), 1.f);
        output.vTangent = float4(input.vTangentWorld, 1.f);
        output.vBitangent = float4(normalize(cross(output.vNormal.xyz, output.vTangent.xyz)), 1.f);
        output.vEmissive = MtrlEmission;
        
        float4 MRAColor = g_btex_1 ? MRA0Tex.Sample(g_LinearWrapSampler, input.vUV0) : (float4) 0.f;
    
        float metallic = g_btex_1 ? MRAColor.x : MtrlMetallic;
        float roughness = g_btex_1 ? MRAColor.y : MtrlRoughness;
        float ao = g_btex_1 ? MRAColor.z : 1.f;
        if (ao >= 1.f)
        {
            ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
        }
        
        output.vMRA = float4(metallic, roughness, ao, 1.f);
    }
    else if (albedo1.a >= 0.1f)
    {
        output.vColor = albedo1;
        output.vPosition = float4(input.vPosWorld, 1.f);
        output.vNormal = float4(g_btex_5 ? NormalMapping(input, Noraml1Tex, input.vUV1, g_LinearWrapSampler) : normalize(input.vNormalWorld), 1.f);
        output.vTangent = float4(input.vTangentWorld, 1.f);
        output.vBitangent = float4(normalize(cross(output.vNormal.xyz, output.vTangent.xyz)), 1.f);
        output.vEmissive = MtrlEmission;
        
        float4 MRAColor = g_btex_4 ? MRA0Tex.Sample(g_LinearWrapSampler, input.vUV1) : (float4) 0.f;
    
        float metallic = g_btex_4 ? MRAColor.x : MtrlMetallic;
        float roughness = g_btex_4 ? MRAColor.y : MtrlRoughness;
        float ao = g_btex_4 ? MRAColor.z : 1.f;
        if (ao >= 1.f)
        {
            ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV1).r;
        }
        
        output.vMRA = float4(metallic, roughness, ao, 1.f);
    }
    else
    {
        output.vColor = input.vColor;
        output.vPosition = float4(input.vPosWorld, 1.f);
        output.vNormal = float4(input.vNormalWorld, 1.f);
        output.vTangent = float4(input.vTangentWorld, 1.f);
        output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
        output.vEmissive = MtrlEmission;
        output.vMRA = float4(MtrlMetallic, MtrlRoughness, SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r, 1.f);
    }

    return output;
}