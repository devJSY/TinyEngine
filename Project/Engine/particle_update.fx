#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "global.hlsli"
#include "struct.hlsli"

RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);

#define MAX_COUNT g_int_0 

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{    
    g_ParticleBuffer[id.x].vWorldPos.y += 100.f * g_dt;
}

#endif