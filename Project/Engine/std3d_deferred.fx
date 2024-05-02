#ifndef _STD_DEFERED
#define _STD_DEFERED

#include "global.hlsli"

// ======================
// Std3D_Deferred Shader
// MRT : Deferred MRT
#define ColorTexture        g_tex_0
#define NormalMap           g_tex_1
#define ColorTextureCheck   g_btex_0
#define NormalMapCheck      g_btex_1
// ======================

PS_IN VS_Std3D_Deferred(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;

    output.vPosProj = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vPosWorld = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.normalWorld = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld));
    output.vTangentWorld = normalize(mul(float4(_in.vTangent, 0.f), g_matWorld));
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vData : SV_Target3;
};

PS_OUT PS_Std3D_Deferred(PS_IN _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;

    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    if (ColorTextureCheck)
    {
        vOutColor = ColorTexture.Sample(g_LinearWrapSampler, _in.vUV);
    }
    
    output.vColor = vOutColor;
    output.vPosition = float4(_in.vPosWorld, 1.f);
        
    float3 vWorldNormal = _in.normalWorld;
    
    if (NormalMapCheck)
    {
        float3 vNormal = NormalMap.Sample(g_LinearWrapSampler, _in.vUV).rgb;
        vNormal = vNormal * 2.f - 1.f;
                        
        float3 N = _in.normalWorld;
        float3 T = normalize(_in.vTangentWorld - dot(_in.vTangentWorld, N) * N);
        float3 B = cross(N, T);
        
        float3x3 TBN = float3x3(T, B, N);
        
        vWorldNormal = normalize(mul(vNormal.xyz, TBN));
    }
        
    output.vNormal = float4(vWorldNormal, 1.f);
    output.vData = float4(0.f, 0.f, 0.f, 1.f);
        
    return output;
}

#endif