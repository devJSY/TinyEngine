#include "global.hlsli"
    
struct GS_Input
{
    float4 posModel : SV_POSITION;
    float3 normalWorld : NORMAL;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

[maxvertexcount(2)]
void main(point GS_Input input[1], inout LineStream<PS_Input> outputStream)
{
    PS_Input output;
    
    float4 posWorld = mul(input[0].posModel, g_matWorld);
    float4 normalModel = float4(input[0].normalWorld, 0.f);
    float4 normalWorld = mul(normalModel, g_matWorldInvTranspose);
    normalWorld = float4(normalize(normalWorld.xyz), 0.f);
    
    // 원본 정점
    output.pos = mul(input[0].posModel, g_matWVP);
    output.color = float3(1.f, 1.f, 0.f);
    outputStream.Append(output);
    
    // 추가된 NormalLine 정점
    output.pos = mul(posWorld + g_NormalLineScale * normalWorld, g_matView);
    output.pos = mul(output.pos, g_matProj);
    output.color = float3(1.f, 0.f, 0.f);
    outputStream.Append(output);
}
