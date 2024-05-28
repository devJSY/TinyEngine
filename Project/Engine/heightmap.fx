#ifndef _HEIGHTMAP
#define _HEIGHTMAP

#include "global.hlsli"
#include "func.hlsli"

RWTexture2D<float> HEIGHT_MAP : register(u0); // unordered acess
StructuredBuffer<tRaycastOut> LOCATION : register(t16); // 브러쉬 위치(좌상단 기준)

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

//#define BRUSH_TEX   g_texarr_0
#define BRUSH_TEX   g_tex_0
#define STRENGTH    g_float_0  // 브러쉬 강도
#define SCALE       g_vec2_0   // 브러쉬 크기
#define BRUSH_IDX   g_int_2    // 브러쉬 인덱스

[numthreads(32, 32, 1)]
void CS_HeightMap(int3 _iThreadID : SV_DispatchThreadID)
{
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].success)
    {
        return;
    }

    int2 vCenterPos = float2(WIDTH, HEIGHT) * LOCATION[0].vUV;
    int2 vScale = float2(WIDTH, HEIGHT) * SCALE;

    if (_iThreadID.x < vCenterPos.x - (vScale.x / 2) || vCenterPos.x + (vScale.x / 2) < _iThreadID.x
        || _iThreadID.y < vCenterPos.y - (vScale.y / 2) || vCenterPos.y + (vScale.y / 2) < _iThreadID.y)
    {
        return;
    }

    // brush texture 에서 샘플링 할 UV 계산
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);
    
    
    // 배열텍스처 진행 시 사용가능
    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_LinearWrapSampler, float3(vUV, BRUSH_IDX), 0);
    
    // 브러쉬로 부터 알파값 샘플링
    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_LinearWrapSampler, vUV, 0);
    HEIGHT_MAP[_iThreadID.xy] += g_EngineDT * vBrushColor.a * STRENGTH; // 브러쉬 알파값으로 높이 설정
    
    //// cos 그래프로 높이 설정
    //float vDist = (distance(vCenterPos, _iThreadID.xy)) / vScale * PI;
    //HEIGHT_MAP[_iThreadID.xy] += saturate(g_EngineDT * cos(vDist) * STRENGTH);
    
}

#endif