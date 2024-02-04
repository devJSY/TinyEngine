#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    return g_btexcube_0 ? g_texCube_0.Sample(g_LinearWrapSampler, input.vPosWorld) : input.vColor;
}