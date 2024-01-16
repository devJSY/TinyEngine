#include "struct.hlsli"
#include "global.hlsli"

struct PS_Input
{
    float4 vPosProj : SV_POSITION;
    float2 vUV : TEXCOORD;
};

float4 main(PS_Input input) : SV_TARGET
{
    float force = 0.125f; // 왜곡 효과 강도
    float size = 0.5; // 원 전체 사이즈
    float thickness = 0.125; // 원 두께
    float4 color = (float4) 0.f;
    
    size = g_TestSize;

    float ratio = g_RenderResolution.x / g_RenderResolution.y;
    float2 vScreenUV = input.vPosProj.xy / g_RenderResolution.xy;
    //loat2 centerUV = input.vPosProj.xy / g_RenderResolution.xy;
    float2 centerUV = float2(0.5f, 0.5f); // 텍스춰 좌표기준
    
    //float2 scaledUV = (input.vUV - float2(centerUV.x, 0.0)) * float2(ratio, 1.0) + float2(centerUV.x, 0.0f); // aspect ratio 적용
    float2 scaledUV = input.vUV; // aspect ratio 적용

    float OuterCircle = 1.f - smoothstep(size - 0.1f, size, length(scaledUV - centerUV));
    float InnerCircle = smoothstep(size - thickness - 0.1f, size - thickness, length(scaledUV - centerUV));
    
    float mask = OuterCircle * InnerCircle; // 해당 영역만 왜곡 처리
    float2 disp = normalize(scaledUV - centerUV) * force * mask;
    
    color = g_postprocess_Tex.Sample(g_LinearSampler, vScreenUV - disp);
    //color.rgb = mask;
    
    return color;
}
