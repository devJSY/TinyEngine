#ifndef _RAYCAST
#define _RAYCAST

#include "global.hlsli"
#include "func.hlsli"

RWStructuredBuffer<tRaycastOut> OUTPUT : register(u0);

#define HEIGHT_MAP      g_tex_0

#define CAM_POS         g_vec4_0
#define CAM_DIR         g_vec4_1

#define FACE_X          g_int_0
#define FACE_Z          g_int_1

[numthreads(32, 32, 1)]
void CS_Raycast(int3 _iThreadID : SV_DispatchThreadID)
{
    int2 id = _iThreadID.xy;

    if (FACE_X * 2 <= id.x || FACE_Z <= id.y)
    {
        return;
    }

    float3 vPos[3] = { (float3) 0.f, (float3) 0.f, (float3) 0.f };

    if (0 == id.x % 2)
    {
        // ¾Æ·¡ÂÊ »ï°¢Çü        
        // 2
        // | \
        // 0--1        
        vPos[0].x = id.x / 2;
        vPos[0].z = id.y;

        vPos[1].x = vPos[0].x + 1;
        vPos[1].z = id.y;

        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y + 1;
    }
    else
    {
        // À­ÂÊ »ï°¢Çü
        // 1--0
        //  \ |
        //    2  
        vPos[0].x = (id.x / 2) + 1;
        vPos[0].z = id.y + 1;

        vPos[1].x = vPos[0].x - 1;
        vPos[1].z = id.y + 1;

        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y;
    }

    for (int i = 0; i < 3; ++i)
    {
        float2 uv = float2(saturate(vPos[i].x / (float) FACE_X), saturate(1.f - vPos[i].z / (float) FACE_Z));
        vPos[i].y = HEIGHT_MAP.SampleLevel(g_LinearClampSampler, uv, 0).x;
    }

    float3 vCrossPoint = (float3) 0.f;
    float fDist = 0.f;

    if (IntersectsRay(vPos, CAM_POS.xyz, CAM_DIR.xyz, vCrossPoint, fDist))
    {
        OUTPUT[0].vUV = float2(saturate(vCrossPoint.x / (float) FACE_X), saturate(1.f - vCrossPoint.z / (float) FACE_Z));
        OUTPUT[0].fDist = fDist;
        OUTPUT[0].success = 1;
    }
}

#endif