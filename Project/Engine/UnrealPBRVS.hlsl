#include "struct.hlsli"
#include "global.hlsli"

#define HeightTexture g_tex_3
#define HeightScale g_float_0

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0.f;
    
    // Normal 벡터 먼저 변환 (Height Mapping)
    float4 normal = float4(input.vNormal, 0.0f);
    output.normalWorld = mul(normal, g_matWorldInvTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    // Tangent 벡터는 modelWorld로 변환
    float4 tangentWorld = float4(input.vTangent, 0.0f);
    tangentWorld = mul(tangentWorld, g_matWorld);

    float4 pos = float4(input.vPos, 1.0f);
    pos = mul(pos, g_matWorld);
    
    // Height Mapping
    if (g_btex_3)
    {
        // Heightmap은 보통 흑백이라서 마지막에 .r로 float 하나만 사용
        float height = HeightTexture.SampleLevel(g_LinearWrapSampler, input.vUV, 0).r;
        height = height * 2.0 - 1.0;
        pos += float4(output.normalWorld * height * HeightScale, 0.0);
    }

    output.vPosWorld = pos.xyz; // 월드 위치 따로 저장

    pos = mul(pos, g_matView);
    pos = mul(pos, g_matProj);

    output.vPosProj = pos;
    output.vUV = input.vUV;
    output.vTangentWorld = tangentWorld.xyz;

    output.vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

    return output;
}
