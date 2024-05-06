#include "struct.hlsli"
#include "global.hlsli"
#include "disksample.hlsli"
#include "Light.hlsli"
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

#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0

// PBR 원칙
// 1. microfacet 표면 모델을 기반으로 한다
// 2. 에너지를 보존한다.
// 3. 물리적 기반 BRDF를 사용한다. // BRDF - Bidirectional reflectance distribution function
float4 main(PS_IN input) : SV_TARGET
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = GetNormal(input);
    
    float3 albedo = g_btex_0 ? AmbientTex.Sample(g_LinearWrapSampler, input.vUV).rgb 
                                 : MtrlAlbedo.rgb;
    float ao = g_btex_1 ? AOTex.Sample(g_LinearWrapSampler, input.vUV).r : 1.f;
    float metallic = g_btex_4 ? MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).b
                                    : MtrlMetallic;
    float roughness = g_btex_4 ? MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).g 
                                      : MtrlRoughness;
    float3 emission = g_btex_5 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV).rgb
                                     : MtrlEmission.rgb;

    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_Light3DCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo, ao, metallic, roughness, directLighting);
    }
    
    // Rim
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 RimColor = RimLight(input.normalWorld, toEye, RIM_COLOR.rgb, RIM_POWER);
    
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    output = float4(ambientLighting + directLighting + emission + RimColor, 1.0);
    output = clamp(output, 0.0, 1000.0);

    return output;
}
