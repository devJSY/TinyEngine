#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "func.hlsli"

#define ColorTexture g_tex_0
#define AlphaTexture g_tex_1
#define GradationTrexture g_tex_2

#define GradationAlpha g_float_0

float4 main(PS_IN input) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float4 color = g_vAlbedo;
    float alpha = 1.f;
    
    if (g_btex_0)
    {
        float Ratio = ColorTexture.Sample(g_LinearWrapSampler, float2(input.vUV0.x + g_Time, input.vUV0.y - g_Time)).r;
        
        color = lerp(g_vAlbedo, float4(1.f, 1.f, 1.f, 1.f), Ratio);
    }
    
    if (g_btex_1)
    {
        float alphaFromTexture = AlphaTexture.Sample(g_LinearWrapSampler, float2(input.vUV0.x + g_Time, input.vUV0.y - g_Time * 2.f)).r;
        
        float4 colorAlpha = ColorTexture.Sample(g_LinearWrapSampler, float2(input.vUV0.x + g_Time, input.vUV0.y - g_Time)).r;
        alpha = lerp(0.2f, 1.0f, colorAlpha);
        
        alpha *= alphaFromTexture;
    }
    
    if(g_btex_2)
    {
        float Alpha = GradationTrexture.Sample(g_LinearClampSampler, float2(input.vUV0.x, (1.f - input.vUV0.y))).r;
        alpha *= Alpha;
    }
    
    alpha = clamp(alpha, 0.f, 1.f);
    
    alpha = pow(alpha, GradationAlpha);
    
    color.a = alpha;
    
    vOutColor = color;

    return vOutColor;
    
}

