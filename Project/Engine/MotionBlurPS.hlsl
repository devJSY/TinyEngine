#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define MotionVectorTex g_tex_0
#define Thresholud g_float_0

#define NumBlurSample 10

float4 main(PS_IN input) : SV_TARGET
{
    float4 Color = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0);

    // Thresholud
    float4 Velocity = MotionVectorTex.Sample(g_LinearClampSampler, input.vUV0);
    Velocity.xy *= 1.f - g_DT;
    if (dot(Velocity.xy, Velocity.xy) / 2.f < Thresholud)
    {
        return float4(Color.rgb, 1.f);
    }

    Velocity.xy /= (float) NumBlurSample;

    int Cnt = 1;
    float4 BColor = (float4) 0.f;

    // »ùÇÃ¸µ ¼ö ¸¸Å­ ¹Ýº¹
    [unroll]
    for (int i = Cnt; i < NumBlurSample; i++)
    {
        float2 vUV = input.vUV0 - Velocity.xy * (float) i;
    
        // Occlusion Fault 
        float DepthZ = DepthOnlyTex.Sample(g_LinearClampSampler, vUV).r;
        if (Velocity.a <= DepthZ + 0.04f)
        {
            // Velocity ÅØ¼¿ÀÇ Color »ùÇÃ¸µ
            BColor = g_postprocess_Tex.Sample(g_LinearClampSampler, vUV);
            
            Cnt++;
            Color += BColor;
        }
    }

    Color /= (float) Cnt;
      
    return Color;
}