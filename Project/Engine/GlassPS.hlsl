#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
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

#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0

#define ALPHA g_float_2

// PBR 원칙
// 1. microfacet 표면 모델을 기반으로 한다
// 2. 에너지를 보존한다.
// 3. 물리적 기반 BRDF를 사용한다. // BRDF - Bidirectional reflectance distribution function
float4 main(PS_IN input) : SV_TARGET
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = g_btex_5 ? NormalMapping(input, NormalTex, input.vUV0, g_LinearWrapSampler) : normalize(input.vNormalWorld);
  
    float4 albedo0 = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 albedo1 = g_btex_1 ? Albedo1Tex.Sample(g_LinearClampSampler, input.vUV1) : (float4) 0.f;
    float4 albedo2 = g_btex_2 ? Albedo2Tex.Sample(g_LinearClampSampler, input.vUV2) : (float4) 0.f;
    float4 albedo3 = g_btex_3 ? Albedo3Tex.Sample(g_LinearClampSampler, input.vUV3) : (float4) 0.f;
    
    albedo0.rgb *= albedo0.a;
    albedo1.rgb *= albedo1.a;
    albedo2.rgb *= albedo2.a;
    albedo3.rgb *= albedo3.a;
    
    float3 albedo = albedo0.rgb + albedo1.rgb + albedo2.rgb + albedo3.rgb;
    
    if (0.f >= length(albedo))
    {
        albedo = MtrlAlbedo.rgb;
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

    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo, ao, metallic, roughness, directLighting);
    }
    
    // Rim
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 RimColor = RimLight(input.vNormalWorld, toEye, RIM_COLOR.rgb, RIM_POWER);
    
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    output = float4(ambientLighting + directLighting + emission + RimColor, ALPHA);
    output = clamp(output, 0.0, 1000.0);

    return output;
}
