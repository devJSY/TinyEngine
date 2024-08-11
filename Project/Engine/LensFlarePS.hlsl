#include "global.hlsli"
#include "func.hlsli"

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 vUV : TEXCOORD;
    uint TexIndex : SV_InstanceID;
    float Brightness : FOG;
};

float4 main(PS_Input input) : SV_TARGET
{
    float4 color = (float4) 0.f;
    
    input.vUV = AspectRatioCorrection(input.vUV);
    
    if (0 == input.TexIndex && g_btex_0)
    {
        color = g_tex_0.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (1 == input.TexIndex && g_btex_1)
    {
        color = g_tex_1.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (2 == input.TexIndex && g_btex_2)
    {
        color = g_tex_2.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (3 == input.TexIndex && g_btex_3)
    {
        color = g_tex_3.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (4 == input.TexIndex && g_btex_4)
    {
        color = g_tex_4.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (5 == input.TexIndex && g_btex_5)
    {
        color = g_tex_5.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (6 == input.TexIndex && g_btex_6)
    {
        color = g_tex_6.Sample(g_LinearClampSampler, input.vUV);
    }
    else if (7 == input.TexIndex && g_btex_7)
    {
        color = g_tex_7.Sample(g_LinearClampSampler, input.vUV);
    }
    
    if (color.a < 0.1f)
        discard;
    
    color += color * 1.5f;
    
    return float4(color.rgb, color.a * input.Brightness * 0.1f);
}