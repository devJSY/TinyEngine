#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN input) : SV_TARGET
{    
    float4 color = g_vAlbedo;
    
    if (g_btex_0)
        color = g_tex_0.Sample(g_LinearWrapSampler, input.vUV);
    
    if (color.a < 0.1)
        discard;
    
    return color;
}