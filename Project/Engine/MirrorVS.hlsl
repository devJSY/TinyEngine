#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    output.vPosProj = mul(float4(input.vPos, 1.0f), g_matWVP);
    output.vUV0 = input.vUV0;

    return output;
}