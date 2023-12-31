#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    output.vPosProj = float4(input.vPos, 1.0f);
    output.vUV = input.vUV;

    return output;
}