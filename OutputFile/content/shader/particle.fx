#ifndef _PARTICLE
#define _PARTICLE

#include "global.hlsli"
#include "struct.hlsli"

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);

#define Particle g_ParticleBuffer[g_int_0]

struct VS_Input
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct PS_Input
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

PS_Input VS_Particle(VS_Input _in)
{
    PS_Input output = (PS_Input) 0.f;
    
    g_ParticleBuffer[g_int_0].vWorldPos;
    
    float3 vWorldPos = (_in.vPos * Particle.vWorldScale.xyz) + Particle.vWorldPos.xyz;
    
    output.vPosition = mul(mul(float4(vWorldPos, 1.f), g_matView), g_matProj);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Particle(PS_Input _in) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}

#endif