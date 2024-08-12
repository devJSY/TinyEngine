#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define MotionVectorTex g_tex_0

float4 main(PS_IN input) : SV_TARGET
{
    float2 MotionVector = MotionVectorTex.Sample(g_LinearClampSampler, input.vUV0).xy;
    
    float4 Color = (float4) 0.f;

    float2 TexCoord = input.vUV0;

    Color += g_postprocess_Tex.Sample(g_LinearClampSampler, TexCoord) * 0.4;
    TexCoord -= MotionVector;
    Color += g_postprocess_Tex.Sample(g_LinearClampSampler, TexCoord) * 0.3;
    TexCoord -= MotionVector;
    Color += g_postprocess_Tex.Sample(g_LinearClampSampler, TexCoord) * 0.2;
    TexCoord -= MotionVector;
    Color += g_postprocess_Tex.Sample(g_LinearClampSampler, TexCoord) * 0.1;

    return Color;
}