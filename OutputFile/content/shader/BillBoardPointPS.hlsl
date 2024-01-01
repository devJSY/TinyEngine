#include "global.hlsli"

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
    float4 color = g_vAmb;
    
    if (g_UseTexture)
        color = g_tex_0.Sample(g_LinearSampler, input.texCoord);
    
    if (color.a < 0.1)
        discard;
    
    return color;
}