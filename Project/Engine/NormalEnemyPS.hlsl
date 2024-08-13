#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"
#include "UnrealPBRCommon.hlsli"

/*********************
|   Tex0 : EyeBase
|   Tex1 : SkinBase
|   Tex2 : SkinNormal
|   Tex3 : SkinMRA
**********************/

/*********************
|   UV0 : EyeUV
|   UV1 : BodyUV
**********************/

#define Albedo0Tex g_tex_0
#define Albedo1Tex g_tex_1
#define Albedo2Tex g_tex_2
#define Albedo3Tex g_tex_3
#define Albedo4Tex g_tex_4

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define sparklyEffect g_float_0

PS_OUT_DEFERRED main(PS_IN input)
{
    PS_OUT_DEFERRED output = (PS_OUT_DEFERRED) 0.f;
    
    float3 albedo = (float3) 0.f;
    float4 EyeBase = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 SkinBase = g_btex_1 ? Albedo1Tex.Sample(g_LinearClampSampler, input.vUV1) : (float4) 0.f;
    
    EyeBase *= EyeBase.a;
    SkinBase *= SkinBase.a;
   
    albedo = EyeBase.a >= 0.1f ? EyeBase.rgb : SkinBase.rgb;
    
    output.vNormal.xyz = g_btex_2 ? NormalMapping(input, g_tex_2, input.vUV1, g_LinearWrapSampler, true) : normalize(input.vNormalWorld);
    output.vNormal.a = 1.f;
    
    float4 SkinMRA = g_btex_3 ? Albedo3Tex.Sample(g_LinearWrapSampler, input.vUV1) : (float4) 0.f;
    
    float metallic = g_btex_3 ? SkinMRA.r : MtrlMetallic;
    float roughness = g_btex_3 ? SkinMRA.g : MtrlRoughness;
    float ao = g_btex_3 ? SkinMRA.b : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV1).r;
    }
    
    output.vColor = float4(albedo, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = MtrlEmission;
    output.vMRA = float4(metallic, roughness, ao, 1.f);
   
    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}