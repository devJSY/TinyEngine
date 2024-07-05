#include "global.hlsli"

#define Threshold g_float_0

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = g_tex_0.Sample(g_LinearClampSampler, input.vUV0).rgb;
    
    // Relative Luminance : 픽셀의 색이 밝은지 어두운지의 기준값
    float RelativeLuminance = dot(color, float3(0.2126f, 0.7152f, 0.0722f));
    
    // 해당 픽셀의 색상이 설정한 Threshold보다 작다면(어두운 색이라면) 검정색으로 처리
    if (RelativeLuminance < Threshold)
        color = float3(0.f, 0.f, 0.f);
    
    return float4(color, 1.f);
}