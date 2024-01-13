#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = float3(0.f, 0.f, 0.f);
    
    color = g_postprocess_Tex.Sample(g_PointSampler, input.vUV).rgb;
    
    float RelativeLuminance = ((0.2126 * color.r) + (0.7152 * color.g) + (0.0722 * color.b));
    
    if (RelativeLuminance < g_Bloom_Threshold)
        color = float3(0.0, 0.0, 0.0);
    
    return float4(color, 1.0);
}