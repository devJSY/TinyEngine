#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define HeightTexture g_tex_6
#define HeightScale g_float_0

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0.f;
    
    float4 PrevPos = float4(input.vPos, 1.f);
    if (g_iAnim)
    {
        Skinning(input.vPos, PrevPos.xyz, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, 0);
    }
    
    float3 tangentWorld = normalize(mul(float4(input.vTangent, 0.f), g_matWorldInvTranspose).xyz);

    float3 BitangentWorld = normalize(mul(float4(input.vBitangent, 0.f), g_matWorldInvTranspose).xyz);

    // Normal 벡터 먼저 변환 (Height Mapping)
    float3 normalWorld = normalize(mul(float4(input.vNormal, 0.f), g_matWorldInvTranspose).xyz);

    float4 posWorld = mul(float4(input.vPos, 1.0f), g_matWorld);
    
    // Height Mapping
    if (g_btex_6)
    {
        // Heightmap은 보통 흑백이라서 마지막에 .r로 float 하나만 사용
        float height = HeightTexture.SampleLevel(g_LinearWrapSampler, input.vUV0, 0).r;
        height = height * 2.0 - 1.0;
        posWorld += float4(normalWorld * height * HeightScale, 0.0);
    }

    output.vPosProj = mul(posWorld, g_matView);
    output.vPosProj = mul(output.vPosProj, g_matProj);
    
    // Motion Blur
    if (g_bMotionBlur)
    {
        PrevPos = mul(PrevPos, g_matPrevWV);
        PrevPos = mul(PrevPos, g_matProj);
   
        float2 velocity = (output.vPosProj.xy / output.vPosProj.w) - (PrevPos.xy / PrevPos.w);
        output.vMotionVector.xy = velocity * 0.5f;
        output.vMotionVector.y *= -1.f;
        output.vMotionVector.z = output.vPosProj.z;
        output.vMotionVector.w = output.vPosProj.w;
    }
    else
    {
        output.vMotionVector = float4(0.f, 0.f, 0.f, 0.f);
    }
    
    output.vPosWorld = posWorld.xyz; // 월드 위치 따로 저장
    
    output.vNormalWorld = normalWorld;
    output.vTangentWorld = tangentWorld;
    output.vBitangentWorld = BitangentWorld;

    output.vColor = input.vColor;
    
    output.vUV0 = input.vUV0;
    output.vUV1 = input.vUV1;
    output.vUV2 = input.vUV2;
    output.vUV3 = input.vUV3;
    
    return output;
}

PS_IN main_Inst(VS_IN input)
{
    PS_IN output = (PS_IN) 0.f;
    
    float4 PrevPos = float4(input.vPos, 1.f);
    if (g_iAnim)
    {
        Skinning(input.vPos, PrevPos.xyz, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, input.iRowIndex);
    }
     
    float3 tangentWorld = normalize(mul(float4(input.vTangent, 0.0f), input.matWorldInvTranspose).xyz);

    float3 BitangentWorld = normalize(mul(float4(input.vBitangent, 0.0f), input.matWorldInvTranspose).xyz);

    // Normal 벡터 먼저 변환 (Height Mapping)
    float3 normalWorld = normalize(mul(float4(input.vNormal, 0.0f), input.matWorldInvTranspose).xyz);

    float4 posWorld = mul(float4(input.vPos, 1.0f), input.matWorld);
    
    // Height Mapping
    if (g_btex_6)
    {
        // Heightmap은 보통 흑백이라서 마지막에 .r로 float 하나만 사용
        float height = HeightTexture.SampleLevel(g_LinearWrapSampler, input.vUV0, 0).r;
        height = height * 2.0 - 1.0;
        posWorld += float4(normalWorld * height * HeightScale, 0.0);
    }
    
    output.vPosProj = mul(posWorld, input.matViewProj);
    
    // Motion Blur
    if (input.iMotionBlur)
    {
        PrevPos = mul(PrevPos, input.matPrevTransform);
        
        float2 velocity = (output.vPosProj.xy / output.vPosProj.w) - (PrevPos.xy / PrevPos.w);
        output.vMotionVector.xy = velocity * 0.5f;
        output.vMotionVector.y *= -1.f;
        output.vMotionVector.z = output.vPosProj.z;
        output.vMotionVector.w = output.vPosProj.w;
    }
    else
    {
        output.vMotionVector = float4(0.f, 0.f, 0.f, 0.f);
    }
    
    output.vPosWorld = posWorld.xyz; // 월드 위치 따로 저장
    
    output.vNormalWorld = normalWorld;
    output.vTangentWorld = tangentWorld;
    output.vBitangentWorld = BitangentWorld;

    output.vColor = input.vColor;
    
    output.vUV0 = input.vUV0;
    output.vUV1 = input.vUV1;
    output.vUV2 = input.vUV2;
    output.vUV3 = input.vUV3;
    
    return output;
}
