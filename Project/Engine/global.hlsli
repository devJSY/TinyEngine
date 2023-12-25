#ifndef _GLOBAL
#define _GLOBAL

#include "struct.hlsli"

cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;
    row_major Matrix g_matWorldInvTranspose;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
}

cbuffer MATERIAL_CONST : register(b1)
{
    float3 g_ambient;
    float g_shininess;
    float3 g_diffuse;
    float dummy1; 
    float3 g_specular;
    float dummy2;
}

cbuffer GLOBAL : register(b2)
{
    Light g_DirLight;
    Light g_PointLight;
    Light g_SpotLight;
    
    float4 g_eyeWorld;
    
    float g_NormalLineScale;
    bool g_UseTexture;
    float2 padd;
}

// 메쉬 재질 텍스춰들 t0 부터 시작
Texture2D g_AlbedoTex : register(t0);
Texture2D g_AoTex : register(t1);
Texture2D g_NormalTex : register(t2);
Texture2D g_heightTexture : register(t3);
Texture2D g_MetallicRoughnessTex : register(t4);
Texture2D g_EmissiveTex : register(t5);

SamplerState g_LinearSampler : register(s0);
SamplerState g_PointSampler : register(s1);
SamplerState g_AnisotropicSampler : register(s2);

#endif