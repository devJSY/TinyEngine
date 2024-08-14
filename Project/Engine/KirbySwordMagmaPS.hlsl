#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex  g_tex_0
#define MRATex      g_tex_1
#define Albedo2Tex  g_tex_2

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define sparklyEffect g_float_0

PS_OUT_DEFERRED main(PS_IN input)
{
    PS_OUT_DEFERRED output = (PS_OUT_DEFERRED) 0.f;

    float2 moveUV = input.vUV0 + g_EngineTime * 0.2f;
	float4 albedo = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
	float4 magma = g_btex_2 ? Albedo2Tex.Sample(g_LinearWrapSampler, moveUV) : (float4) 0.f;
	float metallic = g_btex_1 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).r : MtrlMetallic;
	float roughness = g_btex_1 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).g  : MtrlRoughness;
	float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
	float3 emission = MtrlEmission.rgb;
    
    // effect (overlay)
    albedo = albedo * 0.5f + lerp(saturate(1 - ((1 - albedo) * (1 - magma) * 2)), saturate(albedo * magma * 2), magma.r) * 0.5f;

    output.vColor = float4(albedo.rgb, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vNormal = float4(g_btex_5 ? NormalMapping(input, g_tex_5, input.vUV0, g_LinearWrapSampler) : normalize(input.vNormalWorld),1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = float4(emission, 1.f);
    output.vMRA = float4(metallic, roughness, ao, 1.f);
    
    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}