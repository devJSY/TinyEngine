#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    float4 tangentWorld = float4(input.vTangent, 0.0f);
    tangentWorld = mul(tangentWorld, g_matWorldInvTranspose);
    
    float4 BitangentWorld = float4(input.vBitangent, 0.0f);
    BitangentWorld = mul(BitangentWorld, g_matWorldInvTranspose);

    // Normal 벡터 먼저 변환 (Height Mapping)
    float4 normalWorld = float4(input.vNormal, 0.0f);
    normalWorld = mul(normalWorld, g_matWorldInvTranspose);
    
    float4 pos = float4(input.vPos, 1.0f);
    
    output.vPosProj = mul(pos, g_matWVP);
    output.vPosWorld = mul(pos, g_matWorld).xyz; // 월드 위치 따로 저장

    output.vUV0 = input.vUV0;
    output.vColor = input.vColor;
    
    output.vTangentWorld = tangentWorld.xyz;
    output.vBitangentWorld = BitangentWorld.xyz;
    output.vNormalWorld = normalWorld.xyz;

    return output;
}