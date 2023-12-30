#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    return g_btex_1 ? g_tex_0.Sample(g_PointSampler, input.vUV) : input.vColor;
}