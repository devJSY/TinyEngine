#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define MotionVectorTex g_tex_0
#define Thresholud g_float_0

#define NumBlurSample 5

float4 main(PS_IN input) : SV_TARGET
{
    // 장면의 렌더링 이미지를 가져옵니다. a 성분에 Z 값이 저장된다.
    float4 Out = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0);

    // 속도 맵에서 속도 벡터와 Z 값을 가져옵니다.
    float4 Velocity = MotionVectorTex.Sample(g_LinearClampSampler, input.vUV0);
    if (dot(Velocity.xy, Velocity.xy) < Thresholud)
    {
        return float4(Out.rgb, 1.f);
    }
    
    Velocity.xy /= (float) NumBlurSample;

    int cnt = 1;
    float4 BColor = (float4) 0.f;

    [unroll]
    for (int i = cnt; i < NumBlurSample; i++)
    {
        float DepthZ = TexcoordToView(DepthOnlyTex, input.vUV0 + -Velocity.xy * (float) i).z;
    
        // 속도 맵의 Z 값과 속도 벡터 방향의 텍셀 위치를 샘플링 한 장면의 렌더링 이미지의 Z 값을 비교합니다. (주의 1)
        if (Velocity.a < DepthZ + 0.04f)
        {
            // 속도 벡터 방향의 텍셀 위치를 참조하여 장면 렌더링 이미지의 색 정보를 얻습니다.
            BColor = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0 + -Velocity.xy * (float) i);
            
            cnt++;
            Out += BColor;
        }
        cnt++;
        Out += BColor;
    }

    Out /= (float) cnt;
      
    return float4(Out.rgb, 1.f);
}