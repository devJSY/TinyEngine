#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
   
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_LinearWrapSampler, input.vUV0);
    }
    
    return vOutColor;
}