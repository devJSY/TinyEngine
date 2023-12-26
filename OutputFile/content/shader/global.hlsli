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

#define MAX_LIGHTS        3
#define LIGHT_OFF         0x00
#define LIGHT_DIRECTIONAL 0x01  
#define LIGHT_POINT       0x02  
#define LIGHT_SPOT        0x04  
//#define LIGHT_SHADOW      0x10

cbuffer GLOBAL : register(b2)
{
    Light Lights[MAX_LIGHTS];
    
    float3 g_eyeWorld;
    float fov;
    
    bool UseRim;
    float3 g_rimColor;
    float g_rimPower;

    float g_NormalLineScale;
    bool g_UseTexture;
    
    int width;
 
    float3 outlineColor;
    float thickness;
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