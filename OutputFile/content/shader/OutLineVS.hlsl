#include "struct.hlsli"
#include "global.hlsli"

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    float4 normal = float4(input.vNormal, 0.0);
    output.normalWorld = mul(normal, g_matWorldInvTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);    
    
    float4 pos = float4(input.vPos, 1.0);
    output.vPosProj = mul(pos, g_matWorld);
    output.vPosWorld = mul(pos, g_matWorld).xyz; // 월드 위치 따로 저장
    
    float dist = length(output.vPosWorld - g_eyeWorld);
    float offset = thickness * dist * fov / width;
    
    output.vPosProj += float4(output.normalWorld * offset, 0.0);

    output.vPosProj = mul(output.vPosProj, g_matView);
    output.vPosProj = mul(output.vPosProj, g_matProj);
    
    output.vUV = input.vUV;
    output.vColor = input.vColor;
    
    return output;
}