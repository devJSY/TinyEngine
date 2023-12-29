#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output;
    float4 pos = float4(input.vPos, 1.0f);
    
    output.vPosProj = mul(pos, g_matWVP);
    output.vPosWorld = mul(pos, g_matWorld).xyz; // 월드 위치 따로 저장

    output.vUV = input.vUV;
    output.vColor = input.vColor;
    
    float4 normal = float4(input.vNormal, 0.0f);
    output.normalWorld = mul(normal, g_matWorldInvTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);

    return output;
}