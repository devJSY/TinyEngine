#ifndef _FUNC
#define _FUNC

#include "global.hlsli"

float3 NormalMapping(PS_IN input, Texture2D NormalTex, float2 vUV, SamplerState Sampler, bool InvertNoramlMapY = false)
{
    float3 normal = NormalTex.Sample(Sampler, vUV).rgb;
    
    // DXT5 Normal Map 예외처리
    bool bDXT5Normal = 0.f >= normal.b ? true : false;
    if (bDXT5Normal)
    {
        normal.b = 1.f;
    }

    normal = 2.0 * normal - 1.0; // 범위 조절 [-1.0, 1.0]
        
    // OpenGL 용 노멀맵일 경우에는 y 방향 반전
    normal.y = InvertNoramlMapY ? -normal.y : normal.y;
        
    float3 T = normalize(input.vTangentWorld);
    float3 B = normalize(input.vBitangentWorld);
    float3 N = normalize(input.vNormalWorld);
        
    // 압축되어있는 노말은 Bitangent 사용 X
    float3x3 TBN = bDXT5Normal ? float3x3(T, T, N) : float3x3(T, B, N);
    
    return normalize(mul(normal, TBN));
}

float4 TexcoordToView(Texture2D tex, float2 texcoord)
{
    float4 posProj;

    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
    posProj.xy = texcoord * 2.0 - 1.0;
    posProj.y *= -1; // 주의: y 방향을 뒤집어줘야 합니다.
    posProj.z = tex.Sample(g_LinearClampSampler, texcoord).r;
    posProj.w = 1.0;

    // ProjectSpace -> ViewSpace
    float4 posView = mul(posProj, g_matProjInv);
    posView.xyz /= posView.w;
    
    return posView;
}

float4 TexcoordToView(Texture2D tex, float2 texcoord, float lod)
{
    float4 posProj;

    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
    posProj.xy = texcoord * 2.0 - 1.0;
    posProj.y *= -1; // 주의: y 방향을 뒤집어줘야 합니다.
    posProj.z = tex.SampleLevel(g_LinearClampSampler, texcoord, lod).r;
    posProj.w = 1.0;

    // ProjectSpace -> ViewSpace
    float4 posView = mul(posProj, g_matProjInv);
    posView.xyz /= posView.w;
    
    return posView;
}

// Z축 회전을 적용하는 함수
float2 RotateUV(float2 uv, float angle)
{
    // UV 좌표를 텍스처의 중심으로 이동 (0.5, 0.5)
    uv -= 0.5;

    // 회전 각도 계산
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    // 회전 행렬 적용
    float2 rotatedUV;
    rotatedUV.x = uv.x * cosTheta - uv.y * sinTheta;
    rotatedUV.y = uv.x * sinTheta + uv.y * cosTheta;

    // 다시 원래 위치로 이동
    rotatedUV += 0.5;

    return rotatedUV;
}

float2 AspectRatioCorrection(float2 uv)
{
    float AspectRatio = g_RenderResolution.x / g_RenderResolution.y;
    uv.x *= AspectRatio;
    uv.x -= abs((1.f - AspectRatio) / 2.f); // 줄어든 비율만큼 이동하여 중앙에 맞춤
    
    return uv;
}

// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_Time * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_Time /*그래프 우측 이동 속도*/)) * 2.f * PI * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)].xyz * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

// 참고: https://github.com/opengl-tutorials/ogl/blob/master/tutorial16_shadowmaps/ShadowMapping.fragmentshader
float random(float3 seed, int i)
{
    float4 seed4 = float4(seed, i);
    float dot_product = dot(seed4, float4(12.9898, 78.233, 45.164, 94.673));
    return frac(sin(dot_product) * 43758.5453);
}

int IntersectsRay(float3 _vertices[3], float3 _vStart, float3 _vDir, out float3 _vCrossPoint, out float _fResult)
{
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;

    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _vDir);

    float3 w0 = _vStart - _vertices[0].xyz;
    float a = -dot(normal, w0);
    float t = a / b;

    _fResult = t;

    float3 p = _vStart + t * _vDir;

    _vCrossPoint = p;

    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }

    return 1;
}

float GetTessFactor(float _Length, int _iMinLevel, int _iMaxLevel, float _MinDistance, float _MaxDistance)
{
    if (_MaxDistance < _Length)
    {
        return 0.f;
    }
    else if (_Length < _MinDistance)
    {
        return _iMaxLevel;
    }
    else
    {
        float fLevel = _iMaxLevel - (_iMaxLevel - _iMinLevel) * ((_Length - _MinDistance) / (_MaxDistance - _MinDistance));

        return fLevel;
    }
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

#endif