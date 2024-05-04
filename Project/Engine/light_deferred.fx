#ifndef _LIGHT_DEFERRED
#define _LIGHT_DEFERRED

#include "global.hlsli"
#include "Light.hlsli"

#define MtrlAlbedo g_vAlbedo
#define MtrlDiffuse g_vDiffuse
#define MtrlSpecular g_vSpecular

// ========================
// Directional Light Shader
// MRT      : LIGHT
// Mesh     : RectMesh
// DS_TYPE  : NO_TEST_NO_WIRTE
// BS_TYPE  : ONE_ONE , 여러개의 빛이 누적될 수 있게

// Parameter
// g_int_0 : Light Idex
// g_tex_0 : PositionTargetTex
// g_tex_1 : NormalTargetTex
// g_tex_2 : DiffuseTargetTex
// g_tex_3 : SpecularTargetTex
// ========================

PS_IN VS_DirLight(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;
    
    output.vPosProj = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DirLight(PS_IN _in) : SV_Target
{
    float4 vWorldPos = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
    
    // x,y,z 전부 0이라면 discard
    if (!any(vWorldPos.xyz))
    {
        discard;
    }
    
    float3 vWorldNormal = normalize(g_tex_1.Sample(g_LinearWrapSampler, _in.vUV).xyz);
    float3 vDiffuse = g_tex_2.Sample(g_LinearWrapSampler, _in.vUV).xyz;
    float3 vSpecular = g_tex_3.Sample(g_LinearWrapSampler, _in.vUV).xyz;
       
    tLightInfo LightColor = (tLightInfo) 0.f;
    CalculateLight3D(g_int_0, vWorldPos.xyz, vWorldNormal, vDiffuse, vSpecular, LightColor);
        
    LightColor.vRadiance.a = 1.f;
    
    return LightColor.vRadiance;
}

#endif