#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = float3(0.0, 0.0, 0.0);
    
    if (g_btex_0)
        color = g_tex_0.Sample(g_PointSampler, input.vUV);
    
    float RelativeLuminance = ((0.2126 * color.r) + (0.7152 * color.g) + (0.0722 * color.b));
    
    if (RelativeLuminance < g_Bloom_threshold)
        color = float3(0.0, 0.0, 0.0);
    
    return float4(color, 1.0);
}