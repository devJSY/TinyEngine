#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define ScrollGrd g_tex_0
#define MaskingTexture g_tex_1
#define AlphaWeight g_float_0
#define ColorWeight g_float_1

#define InitFireColor g_vec4_0
#define EndFireColor g_vec4_1

float4 main(PS_IN _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0 && g_btex_1)
    {   
        float2 ColorUV = _in.vUV0.y - g_EngineTime;
        
        float Alpha = clamp(_in.vColor.r * AlphaWeight + ScrollGrd.Sample(g_LinearClampSampler, _in.vUV0).r, 0.f, 1.f);
        float Color = clamp(_in.vColor.r + ScrollGrd.Sample(g_LinearWrapSampler, _in.vUV0).r + ColorWeight * g_tex_1.Sample(g_LinearWrapSampler, ColorUV).r, 0.f, 1.f);
        
        vOutColor.rgb = lerp(InitFireColor.rgb, EndFireColor.rgb, clamp((1.f - Color),0.f,1.f));
        vOutColor.a = Alpha;
    }
    
    return vOutColor;
}

