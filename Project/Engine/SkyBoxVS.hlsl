#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN)0;
    
    float4 WorldPos = mul(float4(input.vPos, 1.f), g_matWorld);
    
    output.vPosWorld = WorldPos.xyz;
    output.vPosProj = mul(WorldPos, g_matView);
    output.vPosProj = mul(output.vPosProj, g_matProj);

    return output;
}