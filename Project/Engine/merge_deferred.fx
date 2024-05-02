#ifndef _MERGE
#define _MERGE

#include "global.hlsli"

// ===============
// Merge Shader
// MRT : SwapChain
// Mesh : RectMesh
#define ColorTargetTex      g_tex_0
#define ColorTargetCheck    g_btex_0
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
    
    if (ColorTargetCheck)
    {
        vOutColor = ColorTargetTex.Sample(g_LinearWrapSampler, _in.vUV);
    }
    
    return vOutColor;
}





#endif