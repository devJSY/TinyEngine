#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#include "Light.hlsli"

#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float3 albedo = g_tex_0.Sample(g_LinearWrapSampler, input.vUV0).rgb;
    float3 vWorldPos = g_tex_1.Sample(g_LinearWrapSampler, input.vUV0).xyz;
    float3 normalWorld = g_tex_2.Sample(g_LinearWrapSampler, input.vUV0).rgb;
    float3 emission = g_tex_3.Sample(g_LinearWrapSampler, input.vUV0).rgb;
    float metallic = g_tex_4.Sample(g_LinearWrapSampler, input.vUV0).r;
    float roughness = g_tex_4.Sample(g_LinearWrapSampler, input.vUV0).g;
    float ao = g_tex_4.Sample(g_LinearWrapSampler, input.vUV0).b;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    }
    float3 pixelToEye = normalize(g_eyeWorld - vWorldPos);
    
    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    float3 directLighting = g_tex_5.Sample(g_LinearWrapSampler, input.vUV0).rgb;
    
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    output = float4(ambientLighting + directLighting + emission, 1.f);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}

