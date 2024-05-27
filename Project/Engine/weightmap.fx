#ifndef _WEIGHTMAP
#define _WEIGHTMAP

#include "global.hlsli"
#include "func.hlsli"

struct tWeight_4
{
    float arrWeight[4];
};

RWStructuredBuffer<tWeight_4> WEIGHT_MAP : register(u0); // unordered acess
StructuredBuffer<tRaycastOut> LOCATION : register(t16); // 브러쉬 위치(좌상단 기준)

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

//#define BRUSH_TEX   g_texarr_0
#define BRUSH_TEX   g_tex_0
#define SCALE       g_vec2_0   // 브러쉬 크기
#define BRUSH_IDX   g_int_2    // 브러쉬 인덱스

#define WEIGHT_IDX  g_int_3    // 올릴 가중치 위치

[numthreads(32, 32, 1)]
void CS_WeightMap(int3 _iThreadID : SV_DispatchThreadID)
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

    // brush texture 로 높이 설정
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);

    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, float3(vUV, BRUSH_IDX), 0);
    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_LinearWrapSampler, vUV, 0);

    // 2차원 인덱스 좌표를 1차원 인덱스로 계산
    uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;

    // 현재 가중치
    float arrWeight[4] = WEIGHT_MAP[iIdx].arrWeight;

    // 증가 량
    float fAdd = g_EngineDT * vBrushColor.a * 2.f;
    
    // 가중치를 올리고, 나머지 부위를 비율에 맞추어 내린다.
    arrWeight[WEIGHT_IDX] = saturate(arrWeight[WEIGHT_IDX] + fAdd);

    // 초과량
    float fTotal = 0.f;
    for (int i = 0; i < 4; ++i)
    {
        fTotal += arrWeight[i];
    }

    // 총 합이 1을 넘어서는 경우
    if (1.f < fTotal)
    {
        float fOver = fTotal - 1.f;

        // 나머지 가중치의 총합
        fTotal = fTotal - arrWeight[WEIGHT_IDX];

        for (int j = 0; j < 4; ++j)
        {
            if (j == WEIGHT_IDX)
                continue;

            // 초과량을 비율에 맞추어 나누어서, 다른 가중치 쪽에서 뺀다.
            arrWeight[j] = saturate(arrWeight[j] - fOver * (arrWeight[j] / fTotal));
        }
    }

    // 버퍼에 출력
    WEIGHT_MAP[iIdx].arrWeight = arrWeight;
}

#endif