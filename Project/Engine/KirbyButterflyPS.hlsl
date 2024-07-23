#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex  g_tex_0
#define Albedo1Tex  g_tex_1
#define Albedo2Tex  g_tex_2

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define sparklyEffect g_float_0

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

    float2 moveUV = input.vUV0;
    moveUV.x -= g_EngineTime * 0.2f;
    moveUV.y += g_EngineTime * 0.2f;
    float4 albedo = float4(0.98f, 0.38f, 0.2f, 1.f);
    float4 wingDown = g_btex_0 ? Albedo0Tex.Sample(g_LinearWrapSampler, input.vUV0) : (float4) 0.f;
    float4 wingUp = g_btex_1 ? Albedo1Tex.Sample(g_LinearWrapSampler, input.vUV0) : (float4) 0.f;
    float4 magma = g_btex_2 ? Albedo2Tex.Sample(g_LinearWrapSampler, moveUV) : (float4) 0.f;
    float metallic = MtrlMetallic;
    float roughness = MtrlRoughness;
    float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    float3 emission = MtrlEmission.rgb;
    
    // albedo
    albedo = albedo * (1.f - wingDown.r) + float4(1.f, 0.14f, 0.08f, 1.f) * (wingDown.r);
    albedo = albedo * (1.f - wingUp.r * 0.6f) + float4(1.f, 0.8f, 0.05f, 1.f) * (wingUp.r * 0.6f);
    
    // effect (overlay)
    magma = float4(1.f, 0.5f, 0.5f, 1.f) * magma;
    albedo = albedo * 0.6f + lerp(saturate(1 - ((1 - albedo) * (1 - magma) * 2)), saturate(albedo * magma * 2), magma.r) * 0.4f;
    
    output.vColor = float4(albedo.rgb, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vNormal = float4(g_btex_5 ? NormalMapping(input, g_tex_5, input.vUV0, g_LinearWrapSampler) : normalize(input.vNormalWorld), 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = float4(emission, 1.f);
    output.vMRA = float4(metallic, roughness, ao, 1.f);
    
    return output;
}