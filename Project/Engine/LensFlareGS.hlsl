#include "global.hlsli"

#define Spacing g_float_0
#define SunNDCPos g_vec2_0

struct GS_Input
{
    float4 pos : SV_POSITION;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 vUV : TEXCOORD;
    uint TexIndex : SV_InstanceID;
    float Brightness : FOG;
};

// NDC 좌표 기준 크기
static const float HalfWidth[8] = { 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f };

[maxvertexcount(32)]
void main(point GS_Input input[1], uint primID : SV_PrimitiveID,
                              inout TriangleStream<PS_Input> outputStream)
{
    float4 up = float4(0.f, 1.f, 0.f, 0.f);
    float4 right = float4(1.f, 0.f, 0.f, 0.f);
   
    PS_Input output;
    
    float2 SunToCenter = -SunNDCPos;
    SunToCenter = normalize(SunToCenter);
    SunToCenter *= 1.f / Spacing;
    
    [unroll]
    for (int i = 0; i < 8; ++i)
    {
        float4 CenterPos = float4(SunNDCPos + SunToCenter * i, 0.f, 1.f);
        float brightness = 1.f - length(CenterPos); // 화면 중앙으로 갈수록 옅어지게 처리
        
        // LB
        output.pos = CenterPos - HalfWidth[i] * right - HalfWidth[i] * up;
        output.vUV = float2(0.0, 1.0);
        output.TexIndex = i;
        output.Brightness = brightness;
        outputStream.Append(output);

        // LT
        output.pos = CenterPos - HalfWidth[i] * right + HalfWidth[i] * up;
        output.vUV = float2(0.0, 0.0);
        output.TexIndex = i;
        output.Brightness = brightness;
        outputStream.Append(output);
    
        // RB
        output.pos = CenterPos + HalfWidth[i] * right - HalfWidth[i] * up;
        output.vUV = float2(1.0, 1.0);
        output.TexIndex = i;
        output.Brightness = brightness;
        outputStream.Append(output);
    
        // RT
        output.pos = CenterPos + HalfWidth[i] * right + HalfWidth[i] * up;
        output.vUV = float2(1.0, 0.0);
        output.TexIndex = i;
        output.Brightness = brightness;
        outputStream.Append(output);
        
    }
}