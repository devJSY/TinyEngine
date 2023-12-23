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
    float3 ambient;
    float shininess;
    float3 diffuse;
    float dummy1; 
    float3 specular;
    float dummy2;
}

cbuffer GLOBAL : register(b2)
{
    Light DirLight;
    Light PointLight;
    Light SpotLight;
    
    float3 eyeWorld;
    bool useTexture;
}

Texture2D g_tex_0 : register(t0);

SamplerState g_sam_0 : register(s0);

#endif