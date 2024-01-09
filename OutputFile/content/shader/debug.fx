#ifndef _DEBUG
#define _DEBUG

#include "global.hlsli"

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

VS_Output VS_DebugShape(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

static float g_Thickness = 0.1f;

float4 PS_DebugShape(VS_Output _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    vOutColor.a = 1.f;
    
    return vOutColor;
}

#endif