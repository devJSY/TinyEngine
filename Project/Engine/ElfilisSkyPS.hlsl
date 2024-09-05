#include "struct.hlsli"
#include "global.hlsli"

#define MaskingTex g_tex_0
#define NoiseTex g_tex_1
#define ColorMapTex g_tex_2
#define ColorNoiseTex g_tex_4
#define ShadowTex g_tex_3
#define EmissiveTex g_tex_5

#define MaskUVScale g_float_0
#define ColorUVScale g_float_1
#define EmissivePower g_float_2
#define DistortionSpeed g_vec2_0
#define DistortionScale g_vec2_1

float4 main(PS_IN input) : SV_TARGET
{
    float4 OutColor = float4(g_vAlbedo.rgb, 1.f);
    float3 Mask = (float3) 0.f;
    float2 NoiseUV = input.vUV0;
   
    // Noise
    if (g_btex_1)
    {
        NoiseUV.x += g_EngineTime * DistortionSpeed.x;
        NoiseUV.y += g_EngineTime * DistortionSpeed.y;
        
        float2 Noise = NoiseTex.Sample(g_LinearWrapSampler, NoiseUV).rg;
        Noise = (Noise - (float2) 0.5f) * 0.1f;
        
        NoiseUV = input.vUV0 + Noise * DistortionScale;
    }
    
    // Mask
    if (g_btex_0)
    {
        Mask = MaskingTex.Sample(g_LinearWrapSampler, NoiseUV * MaskUVScale).rgb;
    }
    
    // ==========================
    // COLOR
    // ==========================
    float2 ColorNoiseUV = input.vUV0;
    
    // Color Noise
    if (g_btex_4)
    {
        ColorNoiseUV.x += g_EngineTime * DistortionSpeed.x;
        ColorNoiseUV.y += g_EngineTime * DistortionSpeed.y;
        
        float2 Noise = ColorNoiseTex.Sample(g_LinearWrapSampler, ColorNoiseUV * 2.f).rg;
        Noise = (Noise - (float2) 0.5f) * 0.1f;
        
        ColorNoiseUV = input.vUV0 + Noise * DistortionScale;
    }
    
    // Color
    if (g_btex_2)
    {
        float4 ColorMap = ColorMapTex.Sample(g_LinearWrapSampler, ColorNoiseUV * ColorUVScale);
        float MixPower = dot(Mask, float3(1.f, 1.f, 1.f)) / 3.f;
        float3 Color = float3(0.2f, 0.2f, 0.2f) + float3(1.5f, 1.5f, 1.5f) * ColorMap.rgb;
        
        OutColor.rgb = OutColor.rgb * (1.f - MixPower) + Color.rgb * MixPower * 2.f;
    }
    
    // Shadow
    if (g_btex_3)
    {
        float3 ShadowColor = float3(0.f, 0.f, 0.f);
        float4 Shadow = ShadowTex.Sample(g_LinearWrapSampler, NoiseUV * MaskUVScale);
        float MixPower = dot(Shadow.rgb, float3(1.f, 1.f, 1.f)) / 3.f;

        OutColor.rgb = OutColor.rgb * (1.f - MixPower) + ShadowColor * MixPower;
    }
    
    // ==========================
    // Emissive
    // ==========================
    if (g_btex_5)
    {
        float EmissiveRatio = EmissiveTex.Sample(g_LinearWrapSampler, ColorNoiseUV).r;
        EmissiveRatio = pow(abs(EmissiveRatio), EmissivePower);
        
        OutColor.rgb += g_vEmission.rgb * EmissiveRatio;
    }
    
    return OutColor;
}