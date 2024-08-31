#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex g_tex_0

#define InvertNormalMapY g_int_0

#define DashStart g_int_1

#define Progress g_float_0
#define ThreshHold g_float_1
#define BackGroundAlpha g_float_2

#define UVScale1 g_vec2_0
#define UVScale2 g_vec2_1
#define BGTextureOffset g_vec2_2
#define DashAlphaRatio g_vec2_3

#define DashColor g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float4 MaskingColor = Albedo0Tex.Sample(g_LinearWrapSampler, float2(input.vUV0.x * UVScale1.x, (input.vUV0.y - Progress) * UVScale1.y));
    float4 MaskingColor2 = Albedo0Tex.Sample(g_LinearWrapSampler, float2(input.vUV0.x * UVScale2.x + BGTextureOffset.x, (input.vUV0.y - Progress) * UVScale2.y) + BGTextureOffset.y);
    
    if (MaskingColor.a <= 0.1f && MaskingColor2.a <= 0.1f)
        discard;
   
    float4 FinalColor = (float4) 0.f;
    
    // Masking Color1
    float Alpha = DashStart == 1 ? ((MaskingColor.r * ThreshHold) + (1.f - ThreshHold)) * DashAlphaRatio.y : 0.0;;
    float3 Color = DashColor.rgb * Alpha;
    
    float4 output = float4(Color, Alpha);
    FinalColor = output;
    
    // Masking Color2
    float Alpha2 = (MaskingColor2.r * ThreshHold) + (1.f - ThreshHold);
    float3 Color2 = DashColor.rgb * Alpha2;
    float4 output2 = float4(Color2, BackGroundAlpha * DashAlphaRatio.x);
    
    if (Alpha <= 0.1f)
    {
        FinalColor = output2;
    }
    
    return FinalColor;
}