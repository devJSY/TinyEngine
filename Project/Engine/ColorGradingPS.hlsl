#include "global.hlsli"
#include "struct.hlsli"

#define LUT g_tex_0

float3 GetLutColor(float3 colorIN)
{
    float2 LutSize = float2(0.00390625f, 0.0625f); // 1 / float2(256, 16)
    float4 LutUV;
    colorIN = saturate(colorIN) * 15.f;
    LutUV.w = floor(colorIN.b);
    LutUV.xy = (colorIN.rg + 0.5f) * LutSize;
    LutUV.x += LutUV.w * LutSize.y;
    LutUV.z = LutUV.x + LutSize.y;
    
    return lerp(LUT.SampleLevel(g_LinearWrapSampler, LutUV.xy, LutUV.z).rgb, LUT.SampleLevel(g_LinearWrapSampler, LutUV.zy, LutUV.z).rgb, colorIN.b - LutUV.w);
}

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0).rgb;
    
    if (g_btex_0)
    {
        color = GetLutColor(color);
    }

    return float4(color, 1.f);
}