#include "struct.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define LIGHT_INDEX g_int_0

float4 main(PS_IN input) : SV_Target
{
    float3 vWorldPos = g_tex_1.Sample(g_LinearWrapSampler, input.vUV0).xyz;
    
    // x,y,z 전부 0이라면 discard
    if (!any(vWorldPos))
    {
        discard;
    }

    float3 albedo = g_tex_0.Sample(g_LinearWrapSampler, input.vUV0).rgb;
    float3 normalWorld = g_tex_2.Sample(g_LinearWrapSampler, input.vUV0).rgb;
    float metallic = g_tex_3.Sample(g_LinearWrapSampler, input.vUV0).r;
    float roughness = g_tex_3.Sample(g_LinearWrapSampler, input.vUV0).g;
    float ao = g_tex_3.Sample(g_LinearWrapSampler, input.vUV0).b;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    }

    float3 directLighting = float3(0, 0, 0);

    DirectLighting(LIGHT_INDEX, vWorldPos, normalWorld, albedo, ao, metallic, roughness, directLighting);
    
    return float4(directLighting, 1.f);
}

