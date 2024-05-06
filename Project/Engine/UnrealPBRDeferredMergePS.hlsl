#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define AmbientTex g_tex_0
#define PositionTex g_tex_1
#define NormalTex g_tex_2
#define EmissiveTex g_tex_3
#define MetallicRoughnessTex g_tex_4
#define AOTex g_tex_5
#define RadianceTex g_tex_6

#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float3 albedo = AmbientTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    float3 vWorldPos = PositionTex.Sample(g_LinearWrapSampler, input.vUV).xyz;
    float3 normalWorld = NormalTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    float3 emission = EmissiveTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    float metallic = MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).b;
    float roughness = MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).g;
    float ao = AOTex.Sample(g_LinearWrapSampler, input.vUV).r;

    float3 pixelToEye = normalize(g_eyeWorld - vWorldPos);
    
    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    float3 directLighting = RadianceTex.Sample(g_LinearWrapSampler, input.vUV).rgb;

    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    output = float4(ambientLighting + directLighting + emission, 1.f);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}

