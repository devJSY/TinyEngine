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

// NDC ¡¬«• ±‚¡ÿ ≈©±‚
static const float HalfWidth[8] = { 0.5f, 0.35f, 0.25f, 0.3f, 0.2f, 0.3f, 0.5f, 0.7f };

[maxvertexcount(48)]
void main(point GS_Input input[1], uint primID : SV_PrimitiveID,
                              inout TriangleStream<PS_Input> outputStream)
{
    float4 up = float4(0.f, 1.f, 0.f, 0.f);
    float4 right = float4(1.f, 0.f, 0.f, 0.f);
   
    PS_Input output;
    
    float2 SunToCenter = -SunNDCPos;
    float Len = length(SunToCenter) / Spacing;
    SunToCenter = normalize(SunToCenter);
    
    [unroll]
    for (int i = 0; i < 8; ++i)
    {
        float4 CenterPos = float4(SunNDCPos + SunToCenter * i * Len, 0.f, 1.f);
        float brightness = 1.f - (length(SunToCenter * i * Len) / 0.6f); // »≠∏È ¡ﬂæ”ø°º≠ ∏÷æÓ¡ˆ∏È ø∂æÓ¡¸
        
        if (brightness > 0.f)
        {
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