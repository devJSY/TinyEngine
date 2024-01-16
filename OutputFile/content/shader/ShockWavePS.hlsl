#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float force = 0.125f;
    float size = 0.5; // 원 전체 사이즈
    float thickness = 0.05; // 원 두께
    float4 color = (float4) 0.f;
        
    size += g_time * 0.1f;
    thickness += g_time * 0.1f;
    
    float ratio = g_RenderResolution.x / g_RenderResolution.y;
    //float2 vScreenUV = input.vPosProj.xy / g_RenderResolution;
    float2 scaledUV = (input.vUV - float2(0.5f, 0.0)) * float2(ratio, 1.0) + float2(0.5f, 0.0f); // aspect ratio 적용
    float2 center = float2(0.5f, 0.5f);
    float mask = (1.f - smoothstep(size - 0.1f, size, length(scaledUV - center))) * smoothstep(size
    - thickness - 0.1f, size - thickness, length(scaledUV - center));
    float2 disp = normalize(scaledUV - center) * force * mask;
    
    color = g_postprocess_Tex.Sample(g_LinearSampler, scaledUV - disp);
    //color.rgb = mask;
    
    return color;
}
