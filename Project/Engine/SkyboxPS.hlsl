#include "global.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    return g_texCube_0.Sample(g_LinearSampler, input.vPosWorld);
}