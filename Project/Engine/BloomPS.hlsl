#include "global.hlsli"
#include "struct.hlsli"

#define Strength g_float_0
#define Threshold g_float_1

float4 main(PS_IN input) : SV_TARGET
{
    float3 BloomTex = float3(0.f, 0.f, 0.f);
    float3 PostProcessColor = float3(0.f, 0.f, 0.f);
    
    BloomTex = g_Bloom_Tex.Sample(g_PointSampler, input.vUV).rgb;
    PostProcessColor = g_postprocess_Tex.Sample(g_PointSampler, input.vUV).rgb;
    
    // Relative Luminance : 픽셀의 색이 밝은지 어두운지의 기준값
    float RelativeLuminance = dot(PostProcessColor, float3(0.2126f, 0.7152f, 0.0722f));
    
    // 해당 픽셀의 색상이 설정한 Threshold보다 작다면(어두운 색이라면) 검정색으로 처리
    if (RelativeLuminance < Threshold)
        PostProcessColor = float3(0.f, 0.f, 0.f);
    
    // 원본 텍스춰와 후처리 텍스춰를 Combine
    return float4(BloomTex + PostProcessColor * Strength, 1.f);
}