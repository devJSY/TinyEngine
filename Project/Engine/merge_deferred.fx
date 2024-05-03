#ifndef _MERGE
#define _MERGE

#include "global.hlsli"

// ===============
// Merge Shader
// MRT : SwapChain
// Mesh : RectMesh
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
    
    float4 vColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
    float4 vDiffuse = g_tex_1.Sample(g_LinearWrapSampler, _in.vUV);
    float4 Specular = g_tex_2.Sample(g_LinearWrapSampler, _in.vUV);
        
    vOutColor = vColor + vDiffuse + Specular;
    vOutColor.a = 1.f;
    
    return vOutColor;
}

#endif