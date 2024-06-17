#ifndef _GLOBAL
#define _GLOBAL

#include "struct.hlsli"

#define PI 3.1415926535f

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
    float4 g_vAlbedo;
    float4 g_vDiffuse;
    float4 g_vSpecular;
    float g_vMetallic;
    float g_vRoughness;
    float2 mtrlpadd;
    float4 g_vEmission;

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
            
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
    
    int g_btexcube_0;
    int g_btexcube_1;
    
    int g_btexarr_0;
    int g_btexarr_1;
        
    // 3D Animation 정보
    int g_iAnim;
    int g_iBoneCount;
}

#define LIGHT_DIRECTIONAL   0  
#define LIGHT_POINT         1  
#define LIGHT_SPOT          2  

cbuffer ANIM_DATA2D : register(b2)
{
    float2 g_vLeftTop;
    float2 g_vSliceSize;
    float2 g_vBackGround;
    float2 g_vOffset;
    int g_UseAnim2D;
    int g_UseBackGround;
}

cbuffer GLOBAL_DATA : register(b3)
{
    float2 g_RenderResolution;
    float2 g_NoiseTexResolution;
    
    float g_DT;
    float g_Time;
    float g_EngineDT;
    float g_EngineTime;
    
    float3 g_eyeWorld;
    int g_EnableSSAO;
    
    uint g_Light2DCount;
    uint g_LightCount;
        
    int g_DrawAsWireFrame;
    int g_RenderOutline;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

TextureCube g_texCube_0 : register(t8);
TextureCube g_texCube_1 : register(t9);

Texture2DArray g_texArr_0 : register(t10);
Texture2DArray g_texArr_1 : register(t11);

Texture2D g_anim2d_tex : register(t12);

StructuredBuffer<tLightInfo> g_Light2D : register(t13);
StructuredBuffer<tLightInfo> g_Light : register(t14);

Texture2D g_postprocess_Tex : register(t15);

Texture2D g_NoiseTex : register(t16);

Texture2D g_BRDFTex : register(t17);
TextureCube g_DiffuseCube : register(t18);
TextureCube g_SpecularCube : register(t19);

// 파티클 20, 21 번레지스터 사용

StructuredBuffer<tTileInfo> g_TileInfo : register(t22);

Texture2D g_LightDepthMapTex1 : register(t23);
Texture2D g_LightDepthMapTex2 : register(t24);
Texture2D g_LightDepthMapTex3 : register(t25);
Texture2D g_StaticLightDepthMapTex : register(t26);

Texture2D SSAOTex : register(t30);

// Animator Bone Matrix Buffer
StructuredBuffer<Matrix> g_arrBoneMat : register(t31);

// Animator 32,33 사용
// CopyBone 34 사용

SamplerState g_LinearWrapSampler : register(s0);
SamplerState g_LinearClampSampler : register(s1);
SamplerState g_PointSampler : register(s2);
SamplerState g_AnisotropicSampler : register(s3);
SamplerState g_ShadowPointSampler : register(s4);
SamplerComparisonState g_ShadowCompareSampler : register(s5);

#endif