#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define RenderTex g_tex_0

#define FarZ g_float_0
#define PowPower g_float_1

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = g_btex_0 ? RenderTex.Sample(g_LinearClampSampler, input.vUV0).rgb : (float3) 0.f;
    float DepthMaskingViewZ = TexcoordToView(DepthMaskingTex, input.vUV0).z;
    DepthMaskingViewZ /= FarZ;
    
    float bias = 0.1f;
    
    // 마스킹 되어있지않은 부분은 원본 색상으로 렌더링
    if (1.f - DepthMaskingViewZ < bias)
    {
        return float4(color, 1.f);
    }
    
    float DepthOnlyViewZ = TexcoordToView(DepthOnlyTex, input.vUV0).z;
    DepthOnlyViewZ /= FarZ;
    
    // 가려지지 않은 부분은 원본 색상으로 렌더링
    if (DepthMaskingViewZ == DepthOnlyViewZ)
    {
        return float4(color, 1.f);
    }
    
    // 시점과의 거리가 가까울수록 진하게 표시
    color *= pow(abs(DepthMaskingViewZ), PowPower);
    
    return float4(color, 1.f);
}