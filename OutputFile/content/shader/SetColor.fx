#ifndef _SETCOLOR
#define _SETCOLOR

#include "global.hlsli"

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

RWTexture2D<float4> g_TargetTex : register(u0);

// 그룹 하나의 스레드 차원 수
// hlsl 5.0, 하나의 그룹이 가질수 있는 최대 스레드개수 1024개
[numthreads(32, 32, 1)]
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    if (TEX_WIDTH <= id.x || TEX_HEIGHT <= id.y)
    {
        return;
    }
    
    g_TargetTex[id.xy] = float4(g_vec4_0.rgb, 1.f);
}

#endif
