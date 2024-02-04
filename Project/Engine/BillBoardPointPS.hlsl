#include "global.hlsli"

struct PS_Input
{
    float4 pos : SV_POSITION;
    float4 posWolrd : POSITION0;
    float4 center : POSITION1;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_Input input) : SV_TARGET
{
    float4 color = g_vAmb;
    
    if (g_UseTexture && g_btex_0)
        color = g_tex_0.Sample(g_LinearWrapSampler, input.texCoord);
    
    if (color.a < 0.1)
        discard;
    
    return color;
}