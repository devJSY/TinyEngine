#include "struct.hlsli"
#include "global.hlsli"

float4 main(VS_OUT input) : SV_TARGET
{
    // 235, 162, 0 - 밝은 주황색
    return float4(235.0 / 255, 162.0 / 255, 10.0 / 255, 1.0);
}