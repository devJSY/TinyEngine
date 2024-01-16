#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    output.vPosWorld = mul(float4(input.vPos, 1.f), g_matWorld);
    output.vPosProj = mul(float4(input.vPos, 1.f), g_matWVP);
    output.vUV = input.vUV;

    return output;
}