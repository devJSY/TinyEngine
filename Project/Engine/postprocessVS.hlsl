#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    output.vPosProj = float4(input.vPos * 2.f, 1.f);
    output.vUV0 = input.vUV0;

    return output;
}