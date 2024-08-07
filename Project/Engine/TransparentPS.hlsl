#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "UnrealPBRCommon.hlsli"

#define AlbedoTex g_tex_0
#define MRATex g_tex_4 // Metallic, Roughness, Ambient Occlusion
#define NormalTex g_tex_5
#define EmissiveTex g_tex_7

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define UVScale g_float_0

float4 main(PS_IN input) : SV_TARGET
{
    if (UVScale > 0.f)
    {
        input.vUV0 *= 1.f / UVScale;
    }

    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = g_btex_5 ? NormalMapping(input, NormalTex, input.vUV0, g_LinearWrapSampler, true) : normalize(input.vNormalWorld);
  
    float4 albedo = g_btex_0 ? AlbedoTex.Sample(g_LinearWrapSampler, input.vUV0) : (float4) 0.f;
   
    if (albedo.a < 0.1f)
    {
        discard;
    }
    
    if (0.f >= length(albedo))
    {
        albedo.rgb = MtrlAlbedo.rgb;
    }
    
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

    float3 ambientLighting = AmbientLightingByIBL(albedo.rgb, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo.rgb, ao, metallic, roughness, directLighting);
    }
    
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    output = float4(ambientLighting + directLighting + emission, albedo.a);
    output = clamp(output, 0.0, 1000.0);

    return output;
}
