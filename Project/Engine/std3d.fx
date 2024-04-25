#ifndef _STD3D
#define _STD3D

#include "global.hlsli"
#include "struct.hlsli"
#include "Light.hlsli"

PS_IN VS_Std3D(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;
    
    output.vPosProj = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vPosWorld = mul(float4(_in.vPos, 1.f), g_matWorld).rgb;

    output.normalWorld = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld).rgb);
    output.vTangentWorld = normalize(mul(float4(_in.vTangent, 0.f), g_matWorld).rgb);
        
    output.vUV = _in.vUV;
    output.vColor = _in.vColor;
    
    return output;
}


float4 PS_Std3D(PS_IN _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    
    // 물체 색상
    float4 ObjectColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    
    // 출력 텍스쳐가 바인딩 되어있다면, 텍스쳐의 색상을 사용한다.
    if (g_btex_0)
    {
        ObjectColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
    }
   
    float3 vWorldNormal = _in.normalWorld;
    
    // 노말 텍스쳐가 바인딩 되어있다면, 노말맵핑을 진행한다.
    if (g_btex_1 && g_int_0)
    {
        // 색상의 범위는 0~1 이지만, 저장된 값은 방향벡터를 뜻하기 때문에 원래 의도한 값으로 바꾸기 위해서
        // 값의 0 ~ 1 범위를 -1.f ~ 1.f 로 변경한다.
        float3 vNormal = g_tex_1.Sample(g_LinearWrapSampler, _in.vUV).rgb;
        vNormal = vNormal * 2.f - 1.f;
                        
        float3 N = _in.normalWorld;
        float3 T = normalize(_in.vTangentWorld - dot(_in.vTangentWorld, N) * N);
        float3 B = cross(N, T);
        
        float3x3 TBN = float3x3(T, B, N);
        
        vWorldNormal = normalize(mul(vNormal.xyz, TBN));
    }
    
    tLightInfo LightColor = (tLightInfo) 0.f;
    
    for (uint i = 0; i < g_Light3DCount; ++i)
    {
        CalculateLight3D(i, _in.vPosWorld, vWorldNormal, LightColor);
    }
        
    vOutColor.rgb = ObjectColor.rgb * LightColor.vRadiance.rgb;
    
    return vOutColor;
}

#endif