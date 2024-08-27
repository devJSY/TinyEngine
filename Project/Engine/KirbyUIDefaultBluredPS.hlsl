
#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MtrlAlbedo g_vAlbedo
#define BluredAmount g_float_0

float4 main(PS_IN _in) : SV_Target
{
    float2 ScreenUV = _in.vPosProj.xy / g_RenderResolution;
    
    float BluredMask = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0).a;
    float4 Color = g_tex_1.Sample(g_LinearWrapSampler, ScreenUV);
    
    // BluredMask
    float4 BluredMask2 = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0 * g_float_0);
    
    float4 OutPut = (float4) 0.f;
    
    // Simple Blur
    float4 Blured = g_tex_1.Sample(g_LinearWrapSampler, ScreenUV + float2(BluredAmount, 0.0)) * 0.25
    + g_tex_1.Sample(g_LinearWrapSampler, ScreenUV - float2(BluredAmount, 0.0)) * 0.25
    + g_tex_1.Sample(g_LinearWrapSampler, ScreenUV + float2(0.0, BluredAmount)) * 0.25
    + g_tex_1.Sample(g_LinearWrapSampler, ScreenUV - float2(0.0, BluredAmount)) * 0.25;
    
    OutPut = lerp(Color, Blured, BluredMask);
    
    return OutPut;
}