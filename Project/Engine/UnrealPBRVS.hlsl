#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define HeightTexture g_tex_3
#define HeightScale g_float_0

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0.f;
    
    if (g_iAnim)
    {
        Skinning(input.vPos, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, 0);
    }
    
    float4 tangentWorld = float4(input.vTangent, 0.0f);
    tangentWorld = mul(tangentWorld, g_matWorldInvTranspose);
    
    float4 BitangentWorld = float4(input.vBitangent, 0.0f);
    BitangentWorld = mul(BitangentWorld, g_matWorldInvTranspose);

    // Normal 벡터 먼저 변환 (Height Mapping)
    float4 normalWorld = float4(input.vNormal, 0.0f);
    normalWorld = mul(normalWorld, g_matWorldInvTranspose);

    float4 posWorld = float4(input.vPos, 1.0f);
    posWorld = mul(posWorld, g_matWorld);
    
    // Height Mapping
    if (g_btex_3)
    {
        // Heightmap은 보통 흑백이라서 마지막에 .r로 float 하나만 사용
        float height = HeightTexture.SampleLevel(g_LinearWrapSampler, input.vUV, 0).r;
        height = height * 2.0 - 1.0;
        posWorld += float4(normalWorld.xyz * height * HeightScale, 0.0);
    }

    output.vPosProj = mul(posWorld, g_matView);
    output.vPosProj = mul(output.vPosProj, g_matProj);

    output.vPosWorld = posWorld.xyz; // 월드 위치 따로 저장
    output.vColor = input.vColor;
    output.vUV = input.vUV;
    
    output.vTangentWorld = tangentWorld.xyz;
    output.vBitangentWorld = BitangentWorld.xyz;
    output.vNormalWorld = normalWorld.xyz;

    return output;
}

PS_IN main_Inst(VS_IN input)
{
    PS_IN output = (PS_IN) 0.f;
    
    if (g_iAnim)
    {
        Skinning(input.vPos, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, input.iRowIndex);
    }
    
    float4 tangentWorld = float4(input.vTangent, 0.0f);
    tangentWorld = mul(tangentWorld, input.matWorldInvTranspose);
    
    float4 BitangentWorld = float4(input.vBitangent, 0.0f);
    BitangentWorld = mul(BitangentWorld, input.matWorldInvTranspose);

    // Normal 벡터 먼저 변환 (Height Mapping)
    float4 normalWorld = float4(input.vNormal, 0.0f);
    normalWorld = mul(normalWorld, input.matWorldInvTranspose);

    float4 posWorld = float4(input.vPos, 1.0f);
    posWorld = mul(posWorld, input.matWorld);
    
    // Height Mapping
    if (g_btex_3)
    {
        // Heightmap은 보통 흑백이라서 마지막에 .r로 float 하나만 사용
        float height = HeightTexture.SampleLevel(g_LinearWrapSampler, input.vUV, 0).r;
        height = height * 2.0 - 1.0;
        posWorld += float4(normalWorld.xyz * height * HeightScale, 0.0);
    }

    output.vPosProj = mul(posWorld, g_matView);
    output.vPosProj = mul(output.vPosProj, g_matProj);

    output.vPosWorld = posWorld.xyz; // 월드 위치 따로 저장
    output.vColor = input.vColor;
    output.vUV = input.vUV;
    
    output.vTangentWorld = tangentWorld.xyz;
    output.vBitangentWorld = BitangentWorld.xyz;
    output.vNormalWorld = normalWorld.xyz;

    return output;
}
