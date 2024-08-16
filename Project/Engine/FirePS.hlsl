#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MaskingTexture g_tex_0
#define LerpRatio g_float_0

#define InitFireColor g_vec4_0
#define EndFireColor g_vec4_1

float4 main(PS_IN _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    vOutColor.a = 1.f;

    if (g_btex_0)
    {
        float Alpha = MaskingTexture.Sample(g_LinearClampSampler, _in.vUV0).x;
        vOutColor.rgb = lerp(InitFireColor.rgb, EndFireColor.rgb, pow(abs(Alpha), 1.f / LerpRatio));
        vOutColor.a = Alpha;
    }
    
    return vOutColor;
}

