#include "global.hlsli"
#include "struct.hlsli"

#define Strength g_float_0
#define Threshold g_float_1

// =========================
// LDR Bloom
// =========================
float4 main(PS_IN input) : SV_TARGET
{
    float3 color0 = g_tex_0.Sample(g_PointSampler, input.vUV).rgb;
    float3 color1 = g_tex_1.Sample(g_PointSampler, input.vUV).rgb;
    
    // Relative Luminance : 픽셀의 색이 밝은지 어두운지의 기준값
    float RelativeLuminance = dot(color1, float3(0.2126f, 0.7152f, 0.0722f));
    
    // 해당 픽셀의 색상이 설정한 Threshold보다 작다면(어두운 색이라면) 검정색으로 처리
    if (RelativeLuminance < Threshold)
        color1 = float3(0.f, 0.f, 0.f);
    
    // 원본 텍스춰와 후처리 텍스춰를 Combine
    return float4(color0 + color1 * Strength, 1.f);
}