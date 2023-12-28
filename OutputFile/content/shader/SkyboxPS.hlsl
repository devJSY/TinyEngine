#include "global.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    return g_cube_0.Sample(g_LinearSampler, input.vPosWorld);
}