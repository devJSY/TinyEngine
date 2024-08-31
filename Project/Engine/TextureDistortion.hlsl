#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MaskTex g_tex_0
#define NoiseTex g_tex_1

#define DistortionSpeed g_vec2_0
#define DistortionScale g_vec2_1
#define VtxScaleColor g_vec4_0

#define MtrlAlbedo g_vAlbedo

float4 main(PS_IN input) : SV_Target
{
    float4 vOutColor = MtrlAlbedo;
    float2 NoiseUV = input.vUV0;
    
    // Get UV
    if (g_btex_1)
    {
        NoiseUV.x +=  g_EngineTime * DistortionSpeed.x;
        NoiseUV.y +=  g_EngineTime * DistortionSpeed.y;
        
        float2 Noise = NoiseTex.Sample(g_LinearWrapSampler, NoiseUV).rg;
        Noise = (Noise - (float2) 0.5f) * 0.1f;
        
        NoiseUV = input.vUV0 + Noise * DistortionScale;
    }
    
    // Get Color
    if (g_btex_0)
    {
        float4 SampleColor = MaskTex.Sample(g_LinearWrapSampler, NoiseUV);
        vOutColor *= SampleColor;
    }
    
    return vOutColor;
}