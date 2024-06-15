#ifndef _COPYBONE
#define _COPYBONE

#include "global.hlsli"

RWStructuredBuffer<matrix> g_arrDestMat : register(u0); // 복사받을 목적지 버퍼
StructuredBuffer<matrix> g_srcMat : register(t34); // 원본 버퍼

// ======================
// BoneMatrix 복사 쉐이더
#define BoneCount   g_int_0
#define RowIndex    g_int_1
// ======================

[numthreads(1024, 1, 1)]
void CS_CopyBoneMatrix(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_int_0)
        return;

    g_arrDestMat[BoneCount * RowIndex + _iThreadIdx.x] = g_srcMat[_iThreadIdx.x];
}

#endif