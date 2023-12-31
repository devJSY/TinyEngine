#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float3 color0 = float3(0.0, 0.0, 0.0);
    float3 color1 = float3(0.0, 0.0, 0.0);
    
    if (g_btex_0)
        color0 = g_tex_0.Sample(g_PointSampler, input.vUV).rgb;
      
    if (g_btex_1)
        color1 = g_tex_1.Sample(g_PointSampler, input.vUV).rgb;
    
    return float4(color0 + g_Bloom_Strength * color1, 1.0);
}