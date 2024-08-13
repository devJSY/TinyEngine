#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"
#include "UnrealPBRCommon.hlsli"

#define Albedo0Tex g_tex_0
#define Albedo1Tex g_tex_1
#define Albedo2Tex g_tex_2
#define Albedo3Tex g_tex_3
#define Albedo4Tex g_tex_4

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic 0.f
#define MtrlRoughness 0.95f
#define MtrlEmission g_vEmission

#define sparklyEffect g_float_0

PS_OUT_DEFERRED main(PS_IN input)
{
    PS_OUT_DEFERRED output = (PS_OUT_DEFERRED) 0.f;

    float3 albedo = float3(1.f, 0.18f, 0.37f);
    float4 eyeBase = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 eyeMask = g_btex_1 ? Albedo1Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 skin = g_btex_3 ? Albedo3Tex.Sample(g_LinearClampSampler, input.vUV2) : (float4) 0.f;
    float4 mouth = g_btex_4 ? Albedo4Tex.Sample(g_LinearClampSampler, input.vUV1) : (float4) 0.f;
    
    // skin
    albedo = albedo * (1.f - skin.r) + float3(1.f, 0.05f, 0.12f) * skin.r;
    albedo = albedo * (1.f - skin.g) + float3(0.67f, 0.f, 0.f) * skin.g;
    
    // mouse
    albedo = albedo * (1.f - mouth.a) + mouth.rgb * mouth.a;
    
    //eye
    float EyeAlpha = dot(eyeBase.rgb, float3(1.0, 1.0, 1.0)) / 3.f;
    
    albedo = albedo * (1.f - EyeAlpha) + (float3) eyeMask.r * EyeAlpha;
    albedo.b += eyeMask.b * EyeAlpha;
    if (EyeAlpha > 0.9f && eyeMask.r >= 0.01f && eyeMask.r <= 1.f)
    {
        albedo.b = clamp(sin(eyeMask.r * PI / 2.f) * 20.f, 0.f, 1.f);
    }
    
    // normal
    if (dot(eyeBase.rgb, float3(1.0, 1.0, 1.0)) / 3.f > 0.99f)
    {
        output.vNormal = float4(g_btex_2 ? NormalMapping(input, Albedo2Tex, input.vUV0, g_LinearClampSampler) : normalize(input.vNormalWorld), 1.f);
    }
    else
    {
        output.vNormal = float4(normalize(input.vNormalWorld), 1.f);
    }
    
    float metallic = MtrlMetallic;
    float roughness = MtrlRoughness;
    float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    float3 emission = MtrlEmission.rgb + float3(sparklyEffect, sparklyEffect, sparklyEffect);

    output.vColor = float4(albedo, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = float4(emission, 1.f);
    output.vMRA = float4(metallic, roughness, ao, 1.f);
    
    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}