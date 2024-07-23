#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

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
    
    float3 albedo = (float3) 0.f;
    float4 EyeColor = g_btex_0 ? g_tex_0.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 SkinColor = g_btex_1 ? g_tex_1.Sample(g_LinearWrapSampler, input.vUV1) : (float4) 0.f;
    
    EyeColor *= EyeColor.a;
    SkinColor *= SkinColor.a;
   
    albedo = EyeColor.a >= 0.1f ? EyeColor.rgb : SkinColor.rgb;
    
    output.vNormal.xyz = g_btex_3 ? NormalMapping(input, g_tex_3, input.vUV0, g_LinearWrapSampler) : normalize(input.vNormalWorld);
    output.vNormal.a = 1.f;
    
    float4 MRAColor = g_btex_2 ? g_tex_2.Sample(g_LinearWrapSampler, input.vUV1) : (float4) 0.f;
    
    float metallic = g_btex_2 ? MRAColor.x : MtrlMetallic;
    float roughness = g_btex_2 ? MRAColor.y : MtrlRoughness;
    float ao = g_btex_2 ? MRAColor.z : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV1).r;
    }

    output.vColor = float4(albedo, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(output.vNormal.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = MtrlEmission;
    output.vMRA = float4(metallic, roughness, ao, 1.f);
   
    return output;
}