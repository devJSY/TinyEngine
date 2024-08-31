#include "struct.hlsli"
#include "global.hlsli"

#define NoiseTex g_tex_0
#define MaskingTex g_tex_1
#define VtxScaleColor g_vec4_0

float4 main(PS_IN input) : SV_TARGET
{
    float4 Color = (float4) 0.f;
    Color.a = 1.f;
        
    float2 ScreenUV = input.vPosProj.xy / g_RenderResolution.xy;
    
    if (g_btex_0)
    {
        float2 MoveUV = input.vUV0;
        
        // Distortion
        MoveUV.x += abs(cos((ScreenUV.x + (g_EngineTime * 0.2f)) * 5.f) * 0.2f);
        MoveUV.y += abs(sin((ScreenUV.y + (g_EngineTime * 0.2f)) * 5.f) * 0.2f);
        
        float2 Noise = NoiseTex.Sample(g_LinearWrapSampler, MoveUV).rg;
        Noise = (Noise.xy - 0.5f) * 0.1f;
        
        // Distortion Masking
        float DistortionMasking = 1.f;
        
        if (g_btex_1)
        {
            float4 Mask = MaskingTex.Sample(g_LinearWrapSampler, input.vUV0);
            DistortionMasking = (Mask.r < Mask.a) ? Mask.r : Mask.a;
        }
        
        // Distortion Scale (by Vtx Color)
        float DistortionScale = 1.f;
        
        if (length(VtxScaleColor) > 0.f)
        {
            // Vtx Color가 검은색에 가까울수록 Distortion이 적게 들어감
            // Vtx Color가 흰색에 가까울수록 Distortion이 원본값으로 들어감
            DistortionScale = dot(input.vColor.rgb * VtxScaleColor.rgb, float3(1.f, 1.f, 1.f)) / 3.f;
        }
        
        ScreenUV += Noise * DistortionScale * DistortionMasking;
    }
        
    Color = g_postprocess_Tex.Sample(g_LinearClampSampler, ScreenUV);
    return Color;
}