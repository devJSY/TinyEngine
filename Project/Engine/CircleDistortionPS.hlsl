#include "struct.hlsli"
#include "global.hlsli"

#define DistortionForce g_float_0 // 왜곡 효과 강도
#define CircleSize g_float_1 // 원 전체 사이즈
#define Thickness g_float_2 // 원 두께

float4 main(PS_IN input) : SV_TARGET
{
    float offsetSize = 0.125f; // 기본 원 사이즈
    float size = offsetSize + CircleSize; // 원 전체 사이즈
    float4 color = (float4) 0.f;

    float ratio = g_RenderResolution.x / g_RenderResolution.y;
    float2 vScreenUV = input.vPosProj.xy / g_RenderResolution.xy;
    float2 centerUV = float2(0.5f, 0.5f);
    
    float2 scaledUV = (input.vUV0 - float2(centerUV.x, 0.0)) * float2(ratio, 1.0) + float2(centerUV.x, 0.0f); // aspect ratio 적용 버전

    float OuterCircle = 1.f - smoothstep(size - 0.1f, size, length(scaledUV - centerUV));
    
    float InnerCircle = smoothstep(size - Thickness - 0.1f, size - Thickness, length(scaledUV - centerUV));
    
    float mask = OuterCircle * InnerCircle; // 해당 영역만 왜곡 처리
    float2 disp = normalize(scaledUV - centerUV) * DistortionForce * mask;
    
    color = g_postprocess_Tex.Sample(g_LinearClampSampler, vScreenUV - disp);
    //color.rgb = mask;  마스킹 확인용
    
    return color;
}
