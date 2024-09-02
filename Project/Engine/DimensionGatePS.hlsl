#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "UnrealPBRCommon.hlsli"

#define MaskingAlphaTex g_tex_0
#define ColorTex g_tex_1 
#define BaseColorNosieTex g_tex_2
#define DistortionNoiseTex g_tex_3

#define ColorCoeff g_float_2
#define DistortionSpeed g_float_1
#define DistortionScale g_float_3
#define NoiseColor g_vec4_0

#define MtrlAlbedo g_vAlbedo
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

float4 main(PS_IN input) : SV_Target
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = input.vNormalWorld;
  
    // -------------
    // Nosie UV
    // -------------
    float2 NoiseUV = input.vUV0;
    
    if (g_btex_3)
    {
        float2 UVDir = normalize(float2(0.5f, 0.5f) - input.vUV0);
        float DistPower = length(UVDir / float2(0.5f, 0.5f)) * DistortionScale;
        NoiseUV = input.vUV0 + UVDir * DistPower + g_EngineTime * DistortionSpeed;
        
        float2 Noise = DistortionNoiseTex.Sample(g_LinearWrapSampler, NoiseUV).rg;
        Noise = (Noise - (float2) 0.5f) * 0.1f;
        
        NoiseUV = input.vUV0 + Noise;
    }
    
    // ------------------
    // Cut by Masking Tex
    // ------------------
    float4 MaskingTex = (float4) 0.f;
    
    if (g_btex_0)
    {
        MaskingTex = MaskingAlphaTex.Sample(g_LinearWrapSampler, NoiseUV);
        
        if (MaskingTex.a <= 0.f)
        {
            clip(-1);
            return (float4) 0.f;
        }
    }
    
    // ------------------
    // get Color
    // ------------------
    float4 albedo = MtrlAlbedo;
    
    // base color noise
    if (g_btex_2)
    {
        float2 MoveUV = input.vUV0 + sin(g_EngineTime * 0.2f);
        float4 Color = BaseColorNosieTex.Sample(g_LinearWrapSampler, NoiseUV);
        float MixPower = dot(Color.rgb, float3(1.f, 1.f, 1.f)) / 3.f;
        albedo = albedo * (1.f - MixPower) + NoiseColor * MixPower;
    }
    
    // color tex
    if (g_btex_1)
    {
        float4 Color = ColorTex.Sample(g_LinearWrapSampler, input.vUV0);
        float ColorPower = clamp(MaskingTex.a * ColorCoeff, 0.f, 1.f);
        albedo.rgb = albedo.rgb * (1.f - ColorPower) + Color.rgb * ColorPower;
    }
    
    // 1. get MRA
    float metallic = 0.f;
    float roughness = MtrlRoughness;
    float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    float3 emission = MtrlEmission.rgb;

    // 2. get lighting
    float3 ambientLighting = AmbientLightingByIBL(albedo.rgb, normalWorld, pixelToEye, ao, metallic, roughness);
    ambientLighting = (length(ambientLighting) < length(albedo.rgb * 0.5f)) ? albedo.rgb * 0.5f : ambientLighting;
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo.rgb, ao, metallic, roughness, directLighting);
    }
    
    // 3. get output
    float4 output;
    output = float4(ambientLighting + directLighting + emission, albedo.a);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}