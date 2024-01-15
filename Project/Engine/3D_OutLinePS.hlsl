#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    // 235, 162, 0 - 밝은 주황색
    return float4(235.f / 255.f, 162.f / 255.f, 10.f / 255.f, 1.f);
}