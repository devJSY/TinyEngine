#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define KirbyMasking g_tex_0

#define FarZ 50000

float4 main(PS_IN _in) : SV_Target
{
    float3 color = g_postprocess_Tex.Sample(g_LinearClampSampler, _in.vUV0).rgb;

    //float MaskingColor = g_btex_0 ? KirbyMasking.Sample(g_LinearClampSampler, _in.vUV0).r : 0.f;
    
    float DepthMaskingViewZ = g_btex_0 ? TexcoordToView(KirbyMasking, _in.vUV0).z : 0.f;
    DepthMaskingViewZ /= FarZ;
    
    float bias = 0.1f;
    
    // 마스킹 되어있지않은 부분은 원본 색상으로 렌더링
    if (1.f - DepthMaskingViewZ < bias)
    {
        color = color * 0.5f;
    }
   
    return float4(color, 1.f);
}