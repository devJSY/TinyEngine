#ifndef _PARTICLE_RENDER
#define _PARTICLE_RENDER

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

[maxvertexcount(12)]
void GS_ParticleRender(point VS_Output _in[1], inout TriangleStream<GS_Output> _OutStream)
{
    GS_Output output[4] = { (GS_Output) 0.f, (GS_Output) 0.f, (GS_Output) 0.f, (GS_Output) 0.f };
    GS_Output output_cross[4] = { (GS_Output) 0.f, (GS_Output) 0.f, (GS_Output) 0.f, (GS_Output) 0.f };
        
    // GS 가 담당하는 파티클 정보를 가져온다.    
    tParticle particle = g_ParticleBuffer[_in[0].iInstID];
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
          
    output_cross[0].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[1].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[2].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    output_cross[3].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    
    output_cross[0].vUV = output[0].vUV = float2(0.f, 0.f);
    output_cross[1].vUV = output[1].vUV = float2(1.f, 0.f);
    output_cross[2].vUV = output[2].vUV = float2(1.f, 1.f);
    output_cross[3].vUV = output[3].vUV = float2(0.f, 1.f);
       
    
    // 렌더모듈 기능
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능
        if (g_ParticleModule[0].VelocityAlignment)
        {
            float3 vR = normalize(mul(float4(particle.vVelocity.xyz, 0.f), g_matView).xyz);
            float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
            float3 vU = normalize(cross(vF, vR));
            
            float3x3 vRot =
            {
                vR,
                vU,
                vF
            };
            
            for (int i = 0; i < 4; ++i)
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
                output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
            }
        }
    }
    
    // View 좌표로 이동, 투영행렬 적용
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
        
        output_cross[i].vPosition.xyz += vViewPos.xyz;
        output_cross[i].vPosition = mul(output_cross[i].vPosition, g_matProj);
        
        output[i].iInstID = _in[0].iInstID;
        output_cross[i].iInstID = _in[0].iInstID;
    }
      
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
        
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능
        if (g_ParticleModule[0].VelocityAlignment)
        {
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[2]);
            _OutStream.Append(output_cross[3]);
            _OutStream.RestartStrip();
    
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[1]);
            _OutStream.Append(output_cross[2]);
            _OutStream.RestartStrip();
        }
    }
}

float4 PS_ParticleRender(GS_Output _in) : SV_Target
{
    tParticle particle = g_ParticleBuffer[_in.iInstID];
    tParticleModule module = g_ParticleModule[0];
    
    // 출력 색상
    float4 vOutColor = particle.vColor;
    vOutColor.a = 1.f;
    
    if (g_btex_0)
    {
        float4 vSampleColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
        vOutColor.rgb *= vSampleColor.rgb;
        vOutColor.a = vSampleColor.a;
    }
    
    // 렌더모듈이 켜져 있으면
    if (module.arrModuleCheck[6])
    {
        if (1 == module.AlphaBasedLife) // Normalize Age
        {
            vOutColor.a *= saturate(1.f - clamp(particle.NormalizeAge, 0.f, 1.f));
        }
        else if (2 == module.AlphaBasedLife) // Max Age
        {
            float fRatio = particle.Age / module.AlphaMaxAge;
            vOutColor.a *= saturate(1.f - clamp(fRatio, 0.f, 1.f));            
        }
    }
    
    return vOutColor;
}

#endif