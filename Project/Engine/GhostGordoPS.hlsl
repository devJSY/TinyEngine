#include "struct.hlsli"
#include "global.hlsli"
#include "UnrealPBRCommon.hlsli"
#include "func.hlsli"
#include "Light.hlsli"

/*********************
|   Tex0 : SkinBase
|   Tex1 : SkinNormal
|   Tex2 : SkinMRA
|   Tex3 : SkinEmissive
**********************/

/*********************
|   UV0 : BodyUV
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

#define RIM_COLOR g_vec4_0
#define RIM_POWER g_float_1

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vTangent : SV_Target3;
    float4 vBitangent : SV_Target4;
    float4 vEmissive : SV_Target5;
    float4 vMRA : SV_Target6;
};

PS_OUT main(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float3 albedo = (float3) 0.f;
    float4 SkinBase = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    
    albedo = SkinBase.rgb * SkinBase.a;
    
    output.vNormal.xyz = g_btex_1 ? NormalMapping(input, g_tex_1, input.vUV0, g_LinearWrapSampler, true) : normalize(input.vNormalWorld);
    output.vNormal.a = 1.f;
    
    float4 SkinMRA = g_btex_2 ? g_tex_2.Sample(g_LinearWrapSampler, input.vUV0) : (float4) 0.f;
    
    float metallic = g_btex_2 ? SkinMRA.r : MtrlMetallic;
    float roughness = g_btex_2 ? SkinMRA.g : MtrlRoughness;
    float ao = g_btex_2 ? SkinMRA.b : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    }
    float3 emission = g_btex_3 ? g_tex_3.Sample(g_LinearWrapSampler, input.vUV0).rgb
                                     : MtrlEmission.rgb;
        
    // Rim
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 RimColor = RimLight(input.vNormalWorld, pixelToEye, RIM_COLOR.rgb, RIM_POWER);
    
    output.vColor = float4(albedo, 1.f);
    output.vPosition = float4(input.vPosWorld, 1.f);
    output.vTangent = float4(input.vTangentWorld, 1.f);
    output.vBitangent = float4(normalize(cross(input.vNormalWorld.xyz, input.vTangentWorld.xyz)), 1.f);
    output.vEmissive = MtrlEmission + float4(RimColor, 1.f);
    output.vMRA = float4(metallic, roughness, ao, 1.f);
   
    return output;
}