#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define MotionVectorTex g_tex_0
#define Thresholud g_float_0
#define VelocityIntensity g_float_1 // Velocity 강도
#define VelocityMaxLength g_float_2 // Velocity 최대 길이

#define NumBlurSample 10

float4 main(PS_IN input) : SV_TARGET
{
    float4 Color = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0);

    // Thresholud
    float4 Velocity = MotionVectorTex.Sample(g_LinearClampSampler, input.vUV0);

    // s = (t0 / dt) * m
    float t0 = 1.f / 60.f; // Desired frame time (assuming 60fps)
    float m = VelocityIntensity;
    float s = (t0 / g_DT) * m;

    // V' = (s / Rmax) * V
    float Rmax = VelocityMaxLength;
    Velocity.xy = (s / Rmax) * Velocity.xy;
    
    if (dot(Velocity.xy, Velocity.xy) / 2.f < Thresholud)
    {
        return float4(Color.rgb, 1.f);
    }

    Velocity.xy /= (float) NumBlurSample;

    int Cnt = 1;
    float4 BColor = (float4) 0.f;

    // 샘플링 수 만큼 반복
    [unroll]
    for (int i = Cnt; i < NumBlurSample; i++)
    {
        float2 vUV = input.vUV0 - Velocity.xy * (float) i;
    
        // Occlusion Fault 
        float DepthZ = DepthOnlyTex.Sample(g_LinearClampSampler, vUV).r;
        if (Velocity.a <= DepthZ + 0.04f)
        {
            // Velocity 텍셀의 Color 샘플링
            BColor = g_postprocess_Tex.Sample(g_LinearClampSampler, vUV);
            
            Cnt++;
            Color += BColor;
        }
    }

    Color /= (float) Cnt;
      
    return Color;
}