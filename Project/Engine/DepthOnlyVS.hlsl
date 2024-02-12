#include "global.hlsli"

float4 main(VS_IN input) : SV_Position
{
    return mul(float4(input.vPos, 1.0f), g_matWVP);
}