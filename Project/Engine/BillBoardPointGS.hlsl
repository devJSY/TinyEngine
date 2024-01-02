#include "global.hlsli"

struct GS_IN
{
    float4 pos : SV_POSITION;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 posWolrd : POSITION0;
    float4 center : POSITION1;
    float2 texCoord : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GS_IN input[1], uint primID : SV_PrimitiveID,
                              inout TriangleStream<PS_IN> outputStream)
{
    // 가로세로 100
    float hw = 0.5 * 100.f;
    
    float4 up = float4(0.0, 1.0, 0.0, 0.0);
    float4 front = float4(g_eyeWorld, 1.0) - input[0].pos;
    front.w = 0.0;
    float4 right = normalize(float4(cross(up.xyz, normalize(front.xyz)), 0.0));
    
    PS_IN output;
    
    output.center = input[0].pos;

    // LB
    output.pos = input[0].pos - hw * right - hw * up;
    output.posWolrd = output.pos;
    output.pos = mul(output.pos, g_matView);
    output.pos = mul(output.pos, g_matProj);
    output.texCoord = float2(0.0, 1.0);
    outputStream.Append(output);

    // LT
    output.pos = input[0].pos - hw * right + hw * up;
    output.posWolrd = output.pos;
    output.pos = mul(output.pos, g_matView);
    output.pos = mul(output.pos, g_matProj);
    output.texCoord = float2(0.0, 0.0);
    outputStream.Append(output);
    
    // RB
    output.pos = input[0].pos + hw * right - hw * up;
    output.posWolrd = output.pos;
    output.pos = mul(output.pos, g_matView);
    output.pos = mul(output.pos, g_matProj);
    output.texCoord = float2(1.0, 1.0);
    outputStream.Append(output);
    
    // RT
    output.pos = input[0].pos + hw * right + hw * up;
    output.posWolrd = output.pos;
    output.pos = mul(output.pos, g_matView);
    output.pos = mul(output.pos, g_matProj);
    output.texCoord = float2(1.0, 0.0);
    outputStream.Append(output);
}