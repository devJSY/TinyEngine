#include "struct.hlsli"
#include "func.hlsli"
#include "global.hlsli"
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

#define Progress g_float_0
#define DashColor g_vec4_0

PS_OUT_DEFERRED main(PS_IN input)
{
    PS_OUT_DEFERRED output = (PS_OUT_DEFERRED) 0.f;
 
    input.vUV0 = float2(input.vUV0.x, input.vUV0.y - Progress);
    
    float4 albedo0 = g_btex_0 ? Albedo0Tex.Sample(g_LinearWrapSampler, input.vUV0) : (float4) 0.f;
    
    albedo0.rgb *= albedo0.a;

    if (albedo0.a <= 0.1f)
        discard;
    
    float3 albedo = albedo0.rgb;
    
    float alpha = albedo.r;
    
    float Ratio = albedo0.r * 0.7 + 0.3;
    
    albedo = DashColor * Ratio;
    
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

    output.vColor = float4(albedo, alpha);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vNormal = float4(g_btex_5 ? NormalMapping(input, NormalTex, input.vUV0, g_LinearWrapSampler, InvertNormalMapY) : input.vNormalWorld, 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(input.vBitangentWorld, 1.f);
    output.vEmissive = float4(emission, 0.f);
    output.vMRA = float4(metallic, roughness, ao, alpha);

    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}