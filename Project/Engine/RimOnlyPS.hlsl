#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "UnrealPBRCommon.hlsli"

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define CHANGE_DURATION g_float_2
#define RIM_POWER g_float_1
#define RIM_COLOR1 g_vec4_0
#define RIM_COLOR2 g_vec4_1
#define BACKGROUND_ALPHA g_float_3

float4 main(PS_IN input) : SV_TARGET
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = input.vNormalWorld;
    
    // Rim
    float period = (CHANGE_DURATION <= 0.f) ? 1.f : CHANGE_DURATION * 2.f;
    float AccTime = g_EngineTime % period;
    float Ratio = AccTime / period;
    float Delta1 = (sin(PI * 2.f * Ratio + PI / 2.f) + 1.f) / 2.f; // 0~1·Î ¸ÅÇÎ
    float Delta2 = 1 - Delta1;
    
    float3 RimColor = RIM_COLOR1.rgb * Delta1 + RIM_COLOR2.rgb * Delta2;
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    RimColor = RimLight(input.vNormalWorld, toEye, RimColor, RIM_POWER);
    float RimStrength = dot(RimColor, float3(1.0, 1.0, 1.0)) / 3.f;
    
    // MRA
    float metallic = MtrlMetallic;
    float roughness = MtrlRoughness;
    float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    float3 emission = MtrlEmission.rgb;

    // Lighting
    float3 ambientLighting = AmbientLightingByIBL(MtrlAlbedo.rgb, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);
    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, MtrlAlbedo.rgb, ao, metallic, roughness, directLighting);
    }
    
    float4 output = 0.f;
    float3 BackgroundColor = float3(ambientLighting + directLighting + emission);
    float OutAlpha = max(RimStrength, BACKGROUND_ALPHA);
    output = float4((BackgroundColor + RimColor).rgb, OutAlpha);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}