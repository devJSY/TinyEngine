#ifndef _GLOBAL
#define _GLOBAL

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
    float4 g_vAmb;
    float4 g_vDiff;
    float4 g_vSpec;
    float4 g_vEmv;

    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
            
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    
    int g_btexcube_0;
    int g_btexcube_1;
    
    int g_btexarr_0;
    int g_btexarr_1;
}

#define MAX_LIGHTS        3
#define LIGHT_OFF         0x00
#define LIGHT_DIRECTIONAL 0x01  
#define LIGHT_POINT       0x02  
#define LIGHT_SPOT        0x04  
//#define LIGHT_SHADOW      0x10

// Α¶Έν
struct Light
{
    float3 strength;
    float fallOffStart;
    float3 direction;
    float fallOffEnd;
    float3 position;
    float spotPower;
    
    uint LightType;
    float3 padd;
};

cbuffer GLOBAL : register(b2)
{
    Light Lights[MAX_LIGHTS];
    
    float3 g_eyeWorld;
    int g_UseTexture;
    
    float3 g_rimColor;
    float g_rimPower;

    float g_NormalLineScale;
    
    int UseRim;
    bool DrawAsWireFrame;
    int padd;
}

Texture2D g_Missing_tex : register(t0);

Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);

TextureCube g_texCube_0 : register(t7);
TextureCube g_texCube_1 : register(t8);

Texture2DArray g_texArr_0 : register(t9);
Texture2DArray g_texArr_1 : register(t10);

SamplerState g_LinearSampler : register(s0);
SamplerState g_PointSampler : register(s1);
SamplerState g_AnisotropicSampler : register(s2);

#endif