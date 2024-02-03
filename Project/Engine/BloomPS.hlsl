#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    //float3 RTCopyTex = float3(0.f, 0.f, 0.f);
    //float3 PostProcessColor = float3(0.f, 0.f, 0.f);
    
    //RTCopyTex = g_RTCopy_Tex.Sample(g_PointSampler, input.vUV).rgb;
    //PostProcessColor = g_postprocess_Tex.Sample(g_PointSampler, input.vUV).rgb;
    
    //// Relative Luminance : 픽셀의 색이 밝은지 어두운지의 기준값
    //float RelativeLuminance = dot(PostProcessColor, float3(0.2126f, 0.7152f, 0.0722f));
    
    //// 해당 픽셀의 색상이 설정한 Threshold보다 작다면(어두운 색이라면) 검정색으로 처리
    //if (RelativeLuminance < g_Bloom_Threshold)
    //    PostProcessColor = float3(0.f, 0.f, 0.f);
    
    //// 원본 텍스춰와 후처리 텍스춰를 Combine
    //return float4(RTCopyTex + PostProcessColor * g_Bloom_Strength, 1.f);
    
    return float4(0.f, 0.f, 0.f, 1.f); // Temp
}