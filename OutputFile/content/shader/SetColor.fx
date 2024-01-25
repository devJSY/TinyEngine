#ifndef _SETCOLOR
#define _SETCOLOR

#include "Global.hlsli"

RWTexture2D<float4> g_TargetTex : register(u0);

[numthreads(32, 32, 1)]
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    g_TargetTex[id.xy].rgb = g_vec4_0.rgb;
}


#endif
