#include "struct.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex g_tex_0
#define Albedo1Tex g_tex_1
#define Albedo2Tex g_tex_2
#define Albedo3Tex g_tex_3
#define MRATex g_tex_4 // Metallic, Roughness, Ambient Occlusion
#define NormalTex g_tex_5
#define EmissiveTex g_tex_7

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
    
    float3 albedo0 = g_btex_0 ? Albedo0Tex.Sample(g_LinearWrapSampler, input.vUV0).rgb 
                                 : MtrlAlbedo.rgb;
    float3 albedo1 = g_btex_1 ? Albedo1Tex.Sample(g_LinearWrapSampler, input.vUV1).rgb 
                                 : MtrlAlbedo.rgb;
    float3 albedo2 = g_btex_2 ? Albedo2Tex.Sample(g_LinearWrapSampler, input.vUV2).rgb 
                                 : MtrlAlbedo.rgb;
    float3 albedo3 = g_btex_3 ? Albedo3Tex.Sample(g_LinearWrapSampler, input.vUV3).rgb 
                                 : MtrlAlbedo.rgb;
    float metallic = g_btex_4 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).r
                                    : MtrlMetallic;
    float roughness = g_btex_4 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).g 
                                      : MtrlRoughness;
    float ao = g_btex_4 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).b : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    }
    float3 emission = g_btex_7 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV0).rgb
                                     : MtrlEmission.rgb;

    output.vColor = float4(albedo1, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vNormal = float4(GetNormal(input), 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = float4(emission, 1.f);
    output.vMRA = float4(metallic, roughness, ao, 1.f);
    
    return output;
}