#ifndef _PARTICLE_RENDER
#define _PARTICLE_RENDER

#include "global.hlsli"
#include "struct.hlsli"

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);

#define Particle g_ParticleBuffer[_in.iInstID]

struct VS_Input
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct PS_Input
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

PS_Input VS_ParticleRender(VS_Input _in)
{
    PS_Input output = (PS_Input) 0.f;
    
    g_ParticleBuffer[g_int_0].vWorldPos;
    
    float3 vWorldPos = (_in.vPos * Particle.vWorldScale.xyz) + Particle.vWorldPos.xyz;
    
    output.vPosition = mul(mul(float4(vWorldPos, 1.f), g_matView), g_matProj);
    output.vUV = _in.vUV;
    output.iInstID = _in.iInstID;
    
    return output;
}

float4 PS_ParticleRender(PS_Input _in) : SV_Target
{
    if (!Particle.Active)
    {
        discard;
    }
    
    float4 vOutColor = Particle.vColor;
    vOutColor.a = 1.f;
    
    if (g_btex_0)
    {
        vOutColor *= g_tex_0.Sample(g_LinearSampler, _in.vUV);
    }
    
    return vOutColor;
}

#endif