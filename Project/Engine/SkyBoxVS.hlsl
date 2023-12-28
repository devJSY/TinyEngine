#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output;
    float4 pos = float4(input.vPos, 1.0f);
    
    output.vPosProj = mul(pos, g_matWVP);
    output.vPosWorld = mul(pos, g_matWorld).xyz; // 월드 위치 따로 저장

    return output;
}