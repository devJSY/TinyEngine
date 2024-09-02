#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex g_tex_0
#define Albedo1Tex g_tex_1
#define Albedo2Tex g_tex_2
#define Albedo3Tex g_tex_3
#define MRATex g_tex_4 // Metallic, Roughness, Ambient Occlusion
#define NormalTex g_tex_5
#define EmissiveTex g_tex_7

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define InvertNormalMapY g_int_0
#define RIM_POWER g_float_1
#define RIM_COLOR g_vec4_0
#define TeleportFlag g_vec4_3.x   // (x) Flag(false, Down, Up), (y) Teleport WorldY, (z) Radius
#define TeleportWorldY g_vec4_3.y
#define TeleportRadius g_vec4_3.z

PS_OUT_FORWARD main(PS_IN input)
{
    // Check Teleport (No Render)
    float Alpha = 1.f;
    
    // teleprot Down : 위쪽만 그림
    if (TeleportFlag == 1)
    {
        if (input.vPosWorld.y < TeleportWorldY)
        {
            if (TeleportRadius > 0.f && input.vPosWorld.y >= TeleportWorldY - TeleportRadius)
            {
                Alpha = 1.f - (TeleportWorldY - input.vPosWorld.y) / TeleportRadius;
            }
            else
            {
                clip(-1);
            }
        }
    }
    
    // teleport Up : 아래쪽만 그림
    else if (TeleportFlag == 2)
    {
        if (input.vPosWorld.y > TeleportWorldY)
        {
            if (TeleportRadius > 0.f && input.vPosWorld.y <= TeleportWorldY + TeleportRadius)
            {
                Alpha = 1.f - (input.vPosWorld.y - TeleportWorldY) / TeleportRadius;
            }
            else
            {
                clip(-1);
            }
        }
    }
    
    // Render
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = g_btex_5 ? NormalMapping(input, NormalTex, input.vUV0, g_LinearWrapSampler, InvertNormalMapY) : input.vNormalWorld;
  
    float4 albedo0 = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 albedo1 = g_btex_1 ? Albedo1Tex.Sample(g_LinearClampSampler, input.vUV1) : (float4) 0.f;
    float4 albedo2 = g_btex_2 ? Albedo2Tex.Sample(g_LinearClampSampler, input.vUV2) : (float4) 0.f;
    float4 albedo3 = g_btex_3 ? Albedo3Tex.Sample(g_LinearClampSampler, input.vUV3) : (float4) 0.f;
    
    albedo0.rgb *= albedo0.a;
    albedo1.rgb *= albedo1.a;
    albedo2.rgb *= albedo2.a;
    albedo3.rgb *= albedo3.a;
    
    float3 albedo = albedo0.rgb + albedo1.rgb + albedo2.rgb + albedo3.rgb;
    
    if (0.f >= length(albedo))
    {
        albedo = MtrlAlbedo.rgb;
    }
    
    float metallic = g_btex_4 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).r
                                    : MtrlMetallic;
    float roughness = g_btex_4 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).g 
                                      : MtrlRoughness;
    float ao = g_btex_4 ? MRATex.Sample(g_LinearWrapSampler, input.vUV0).b : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    }
    float3 emission = g_btex_7 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV0).rgb
                                     : MtrlEmission.rgb;

    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo, ao, metallic, roughness, directLighting);
    }
    
    // Rim
    float3 RimColor = RimLight(input.vNormalWorld, pixelToEye, RIM_COLOR.rgb, RIM_POWER);
    
    PS_OUT_FORWARD output;
    output.vColor = float4(ambientLighting + directLighting + emission + RimColor, Alpha);
    output.vColor = clamp(output.vColor, 0.0, 1000.0);

    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}
