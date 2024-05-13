#include "struct.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define AmbientTex g_tex_0
#define AOTex g_tex_1
#define NormalTex g_tex_2
#define MetallicRoughnessTex g_tex_4
#define EmissiveTex g_tex_5

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
    float4 vMetallicRoughness : SV_Target6;
    float4 vAmbientOcclusion : SV_Target7;
};

PS_OUT main(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float3 albedo = g_btex_0 ? AmbientTex.Sample(g_LinearWrapSampler, input.vUV).rgb 
                                 : MtrlAlbedo.rgb;
    float ao = g_btex_1 ? AOTex.Sample(g_LinearWrapSampler, input.vUV).r : 1.f;
    float metallic = g_btex_4 ? MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).b
                                    : MtrlMetallic;
    float roughness = g_btex_4 ? MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).g 
                                      : MtrlRoughness;
    float3 emission = g_btex_5 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV).rgb
                                     : MtrlEmission.rgb;

    output.vColor = float4(albedo, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vNormal = float4(GetNormal(input), 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.normalWorld.xyz, output.vTangent.xyz)), 1.f);
    output.vEmissive = float4(emission, 1.f);
    output.vMetallicRoughness = float4(0.f, roughness, metallic, 1.f);
    output.vAmbientOcclusion = float4(ao, ao, ao, 1.f);
    
    return output;
}