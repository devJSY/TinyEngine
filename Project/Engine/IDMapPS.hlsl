#include "struct.hlsli"
#include "global.hlsli"

#define PickingColor g_vec4_0

float4 main(PS_IN input) : SV_TARGET
{    
    float4 color = PickingColor;
    color.a = 1.f;
    return color;
}