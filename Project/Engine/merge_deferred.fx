#ifndef _MERGE
#define _MERGE

#include "global.hlsli"

#define SpecularIBLTex g_SpecularCube
#define IrradianceIBLTex g_DiffuseCube 

#define MtrlDiffuse g_vDiffuse
#define MtrlSpecular g_vSpecular
 
// ===============
// Merge Shader
// MRT : SwapChain
// Mesh : RectMesh

// Parameter
// g_tex_0 : AlbedoTargetTex
// g_tex_1 : LightRadianceTargetTex
// g_tex_2 : PositionTargetTex
// g_tex_3 : NormalTargetTex
// g_tex_4 : DiffuseTargetTex
// g_tex_5 : SpecularTargetTex
// ===============

struct VS_Input
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Output
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_Output VS_Merge(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Merge(VS_Output _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float4 vAlbedo = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
    float4 vRadiance = g_tex_1.Sample(g_LinearWrapSampler, _in.vUV);
        
    vOutColor = vAlbedo + vRadiance;
    
    // IBL
    float3 vWorldPosition = g_tex_2.Sample(g_LinearWrapSampler, _in.vUV).xyz;
    float3 vWorldNormal = normalize(g_tex_3.Sample(g_LinearWrapSampler, _in.vUV).xyz);
    
    float3 toEye = normalize(g_eyeWorld - vWorldPosition);
    
    float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
    float4 specular = float4(0.0, 0.0, 0.0, 0.0);
    
    diffuse = IrradianceIBLTex.Sample(g_LinearWrapSampler, vWorldNormal);
    diffuse.xyz *= g_tex_4.Sample(g_LinearWrapSampler, _in.vUV).xyz;
    
    specular = SpecularIBLTex.Sample(g_LinearWrapSampler, reflect(-toEye, vWorldNormal));
    specular.xyz *= g_tex_5.Sample(g_LinearWrapSampler, _in.vUV).xyz;

    vOutColor += diffuse + specular;
    vOutColor.a = 1.f;
    
    return vOutColor;
}

#endif