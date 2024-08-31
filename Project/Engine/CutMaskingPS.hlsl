#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "UnrealPBRCommon.hlsli"

#define MaskingAlphaTex g_tex_0
#define ColorTex g_tex_1 
#define MRATex g_tex_2 // Metallic, Roughness, Ambient Occlusion
#define NormalTex g_tex_3
#define EmissiveTex g_tex_4

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define ColorCoeff g_float_2
#define InvertNormalMapY g_int_0
#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = g_btex_3 ? NormalMapping(input, NormalTex, input.vUV0, g_LinearWrapSampler, InvertNormalMapY) : input.vNormalWorld;
  
    // 0. get albedo
    float4 albedo = MtrlAlbedo;
    float4 MaskingTex = (float4) 0.f;
    
    if (g_btex_0)
    {
        MaskingTex = MaskingAlphaTex.Sample(g_LinearWrapSampler, input.vUV0);
        
        if (MaskingTex.a <= 0.f)
        {
            clip(-1);
            return (float4) 0.f;
        }
    }
    
    // get Color
    if (g_btex_1)
    {
        float4 Color = ColorTex.Sample(g_LinearWrapSampler, input.vUV0);
        float ColorPower = clamp(MaskingTex.a * ColorCoeff, 0.f, 1.f);
        albedo.rgb = albedo.rgb * (1.f - ColorPower) + Color.rgb * ColorPower;
    }
    
    // 1. get MRA
    float metallic = g_btex_2 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).r
                                    : MtrlMetallic;
    float roughness = g_btex_2 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).g 
                                      : MtrlRoughness;
    float ao = g_btex_2 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).b : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    }
    float3 emission = g_btex_4 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV0).rgb
                                     : MtrlEmission.rgb;

    // 2. get lighting
    float3 ambientLighting = AmbientLightingByIBL(albedo.rgb, normalWorld, pixelToEye, ao, metallic, roughness);
    ambientLighting = (length(ambientLighting) < length(albedo.rgb * 0.5f)) ? albedo.rgb * 0.5f : ambientLighting;
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo.rgb, ao, metallic, roughness, directLighting);
    }
    
    // 3. get Rim
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 RimColor = RimLight(input.vNormalWorld, toEye, RIM_COLOR.rgb, RIM_POWER);
    
    // 4. get output
    float4 output;
    output = float4(ambientLighting + directLighting + emission + RimColor, albedo.a);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}