#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "UnrealPBRCommon.hlsli"

#define AlbedoTex g_tex_0
#define NoiseTex g_tex_1
#define NoiseTex1 g_tex_4
#define NormalTex g_tex_2
#define EmissiveTex g_tex_3

#define MtrlAlbedo g_vAlbedo
#define MtrlEmission g_vEmission

#define InvertNormalMapY g_int_0
#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0
#define MtrlAlpha g_float_2
#define AlphaThreshhold g_float_3

PS_OUT_FORWARD main(PS_IN input)
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = g_btex_2 ? NormalMapping(input, NormalTex, input.vUV0, g_LinearWrapSampler, InvertNormalMapY) : input.vNormalWorld;
  
    // 1. get Albedo
    float3 albedo = g_btex_0 ? AlbedoTex.Sample(g_LinearClampSampler, input.vUV0) : MtrlAlbedo.rgb;
    float alpha = 1.f;
    
    // alpha : 임계값보다 밝으면 투명하게 처리
    float RelativeLuminance = dot(albedo, float3(0.2126f, 0.7152f, 0.0722f));
    if (RelativeLuminance >= AlphaThreshhold)
    {
        alpha = 1.f - RelativeLuminance;
    }
    
    // color : albedo를 정해둔 색으로 매핑
    if (g_btex_0)
    {
        albedo = MtrlAlbedo.rgb;
    }
    
    // color : nosie tex
    if (g_btex_4)
    {
        float2 moveUV = input.vUV0;
        moveUV.y *= 1.5f;
        moveUV.x += g_EngineTime * 0.3f;
        float noiseAlpha = NoiseTex1.Sample(g_LinearWrapSampler, moveUV).r;
        
        albedo = saturate(albedo + float3(1.f, 0.f, 0.125f) * noiseAlpha);
    }
    
    // alpha : nosie tex
    if (g_btex_1)
    {
        float2 moveUV = input.vUV0;
        moveUV.y *= 1.5f;
        moveUV.x += g_EngineTime * 0.2f;
        float noiseAlpha = NoiseTex.Sample(g_LinearWrapSampler, moveUV).r;
        
        alpha = clamp(alpha - noiseAlpha, 0.f, 1.f);
    }
    
    // alpha : 외곽쪽일수록 불투명하게
    float brightAlpha = 1.f - pow(saturate(abs(dot(-pixelToEye, normalWorld))), 0.1f);
    brightAlpha = brightAlpha * 0.85f + 0.15f; // 최소값: 0.15f, 최대값: 1.f
    alpha = clamp(alpha + brightAlpha, 0.f, 1.f);
    
    alpha *= MtrlAlpha;
    
    // 2. get MRA
    float metallic = 0.f;
    float roughness = 1.f;
    float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    float3 emission = g_btex_3 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV0).rgb
                                     : MtrlEmission.rgb;

    // 3. get Lighitng
    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo, ao, metallic, roughness, directLighting);
    }
    
    // 4. get Rim
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 RimColor = RimLight(normalWorld, toEye, RIM_COLOR.rgb, RIM_POWER);
    
    // 5. get Output
    PS_OUT_FORWARD output;
    output.vColor = float4(ambientLighting + directLighting + emission + RimColor, alpha);
    output.vColor = clamp(output.vColor, 0.0, 1000.0);

    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}
