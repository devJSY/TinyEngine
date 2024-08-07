#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define DepthOnlyTex g_tex_0
#define DepthMaskingTex g_tex_1
#define MaskingColor g_vec4_0

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0).rgb;
    
    float DepthOnlyViewZ = g_btex_0 ? TexcoordToView(DepthOnlyTex, input.vUV0).z : 0.f;
    float DepthMaskingViewZ = g_btex_1 ? TexcoordToView(DepthMaskingTex, input.vUV0).z : 0.f;
    
    float bias = 1e-3f;
    
    // 가려지지 않았다.
    if (abs(DepthMaskingViewZ - DepthOnlyViewZ) < bias)
    {
        return float4(color, 1.f);
    }
    
    // 가려진 거리에 따른 가중치 적용
    float weight = abs(DepthMaskingViewZ - DepthOnlyViewZ) / DepthMaskingViewZ;
    weight = 1.f - weight; // 거리가 멀수록 MaskingColor의 가중치가 높아지도록 설정
    
    return float4((color * weight) + (MaskingColor.rgb * (1.f - weight)), 1.f);
}