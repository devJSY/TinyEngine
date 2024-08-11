#include "global.hlsli"
#include "func.hlsli"

#define bDepthCheck g_int_0
#define Spacing g_float_0
#define SunNDCPos g_vec4_0
#define CenterOffset g_vec2_1

struct GS_Input
{
    float4 pos : SV_POSITION;
    float3 PosView : POSITION;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 vUV : TEXCOORD;
    uint TexIndex : SV_InstanceID;
    float Brightness : FOG;
};

// NDC 좌표 기준 크기
static const float HalfWidth[8] = { 0.4f, 0.25f, 0.15f, 0.2f, 0.1f, 0.2f, 0.4f, 0.6f };

[maxvertexcount(48)]
void main(point GS_Input input[1], uint primID : SV_PrimitiveID,
                              inout TriangleStream<PS_Input> outputStream)
{
    bool bDirtyFlag = false;
    
    if (bDepthCheck)
    {
        // NDC → UV 
        float2 SunUV = float2(SunNDCPos.x, -SunNDCPos.y);
        SunUV += 1.f;
        SunUV *= 0.5f;
    
        float ViewZ = TexcoordToView(DepthOnlyTex, SunUV, 0).z;
        
        // Sun을 가로 막고있지 않는 경우에만 생성
        bDirtyFlag = input[0].PosView.z >= ViewZ;
    }
    
    if (!bDirtyFlag)
    {
        float4 up = float4(0.f, 1.f, 0.f, 0.f);
        float4 right = float4(1.f, 0.f, 0.f, 0.f);
   
        PS_Input output;
    
        float2 SunToCenter = CenterOffset - SunNDCPos.xy;
        float brightness = 1.f - length(SunToCenter); // Sun 으로부터의 거리가 멀어질수록 밝아지도록 설정
        float Len = length(SunToCenter) / Spacing;
        SunToCenter = normalize(SunToCenter);
    
        if (brightness > 0.f)
        {
            [unroll]
            for (int i = 0; i < 8; ++i)
            {
                float4 CenterPos = float4(SunNDCPos.xy + SunToCenter * i * Len, 0.f, 1.f);
        
                // LB
                output.pos = CenterPos - HalfWidth[i] * right - HalfWidth[i] * up;
                output.vUV = float2(0.f, 1.f);
                output.TexIndex = i;
                output.Brightness = brightness;
                outputStream.Append(output);

                // LT
                output.pos = CenterPos - HalfWidth[i] * right + HalfWidth[i] * up;
                output.vUV = float2(0.f, 0.f);
                output.TexIndex = i;
                output.Brightness = brightness;
                outputStream.Append(output);
    
                // RB
                output.pos = CenterPos + HalfWidth[i] * right - HalfWidth[i] * up;
                output.vUV = float2(1.f, 1.f);
                output.TexIndex = i;
                output.Brightness = brightness;
                outputStream.Append(output);
        
                outputStream.RestartStrip();
    
                // LT
                output.pos = CenterPos - HalfWidth[i] * right + HalfWidth[i] * up;
                output.vUV = float2(0.f, 0.f);
                output.TexIndex = i;
                output.Brightness = brightness;
                outputStream.Append(output);
    
                // RT
                output.pos = CenterPos + HalfWidth[i] * right + HalfWidth[i] * up;
                output.vUV = float2(1.f, 0.f);
                output.TexIndex = i;
                output.Brightness = brightness;
                outputStream.Append(output);
        
                // RB
                output.pos = CenterPos + HalfWidth[i] * right - HalfWidth[i] * up;
                output.vUV = float2(1.f, 1.f);
                output.TexIndex = i;
                output.Brightness = brightness;
                outputStream.Append(output);
        
                outputStream.RestartStrip();
            }
        }
    }
}