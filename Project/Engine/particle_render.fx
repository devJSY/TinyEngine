#ifndef _PARTICLE
#define _PARTICLe

#include "global.hlsli"
#include "struct.hlsli"

StructuredBuffer<tParticle> g_ParticleBuffer : register(t20);
StructuredBuffer<tParticleModule> g_ParticleModule : register(t21);

#define Particle g_ParticleBuffer[_in.iInstID]

struct VS_Input
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct VS_Output
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

VS_Output VS_ParticleRender(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.iInstID = _in.iInstID;
    
    return output;
}

// Geometry Shader
// 정점을 생성시킬 수 있는 쉐이더
// 1. 담당 파티클이 비활성화 상태인 경우, 렌더링을 정점연산 단계에서 중단시키기
// 2. 빌보드 구현의 편의성
struct GS_Output
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_ParticleRender(point VS_Output _in[1], inout TriangleStream<GS_Output> _OutStream)
{
    GS_Output output[4] = { (GS_Output) 0.f, (GS_Output) 0.f, (GS_Output) 0.f, (GS_Output) 0.f };
    
    // GS 가 담당하는 파티클 정보를 가져온다.
    tParticle particle = g_ParticleBuffer[(int) _in[0].iInstID];
    if (0 == particle.Active)
    {
        return;
    }
    
    // 파티클의 중심점을 월드로 이동    
    float3 vWorldPos = particle.vWorldPos.xyz;
    
    // 파티클의 ViewSpace 상에서의 중심 포지션 구하기
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    output[0].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
        
    // 렌더모듈 기능
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능
        if (g_ParticleModule[0].VelocityAlignment)
        {
            // 파티클 월드 기준 속도를 View 공간으로 변환
            float3 vVelocity = normalize(particle.vVelocity);
            vVelocity = mul(float4(vVelocity, 0.f), g_matView).xyz;
                       
            // 파티클 Right 방향과 이동 방향을 내적해서 둘 사이의 각도를 구한다.
            float3 vRight = float3(1.f, 0.f, 0.f);
            float fTheta = acos(dot(vRight, vVelocity));
            
            // 내적의 결과가 코사인 예각을 기준으로 하기 때문에, 2파이 에서 반대로 뒤집어 준다.
            if (vVelocity.y < vRight.y)
            {
                fTheta = (2.f * PI) - fTheta;
            }
            
            // 구한 각도로 Z 축 회전 행렬을 만든다.
            float3x3 matRotZ =
            {
                cos(fTheta), sin(fTheta),  0,
                -sin(fTheta), cos(fTheta), 0,
                      0,           0,      1.f,
            };
            
            // 4개의 정점을 회전시킨다.
            for (int i = 0; i < 4; ++i)
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, matRotZ);
            }
        }
    }
        
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
    }
    
    // 투영행렬 적용
    output[0].vPosition = mul(output[0].vPosition, g_matProj);
    output[1].vPosition = mul(output[1].vPosition, g_matProj);
    output[2].vPosition = mul(output[2].vPosition, g_matProj);
    output[3].vPosition = mul(output[3].vPosition, g_matProj);
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
}

float4 PS_ParticleRender(GS_Output _in) : SV_Target
{
    float4 vOutColor = g_ParticleBuffer[(uint) _in.iInstID].vColor;
    
    if (g_btex_0)
    {
        vOutColor *= g_tex_0.Sample(g_LinearSampler, _in.vUV);
    }
    
    return vOutColor;
}

#endif