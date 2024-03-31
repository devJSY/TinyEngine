#include "global.hlsli"

#define Strength g_float_0

float4 main(PS_IN input) : SV_TARGET
{
    float3 color0 = g_tex_0.Sample(g_PointSampler, input.vUV).rgb; 
    float3 color1 = g_tex_1.Sample(g_PointSampler, input.vUV).rgb;

    return float4(color0 + Strength * color1, 1.f);
}