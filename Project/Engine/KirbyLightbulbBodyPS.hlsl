#include "struct.hlsli"
#include "global.hlsli"
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

PS_OUT_FORWARD main(PS_IN input)
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = (float3) 0.f;
    
    float3 albedo = float3(1.f, 0.18f, 0.37f);
    float4 eyeBase = g_btex_0 ? Albedo0Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 eyeMask = g_btex_1 ? Albedo1Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 eyeNormal = g_btex_2 ? Albedo2Tex.Sample(g_LinearClampSampler, input.vUV0) : (float4) 0.f;
    float4 skin = g_btex_3 ? Albedo3Tex.Sample(g_LinearClampSampler, input.vUV2) : (float4) 0.f;
    float4 mouth = g_btex_4 ? Albedo4Tex.Sample(g_LinearClampSampler, input.vUV1) : (float4) 0.f;
    
    // skin
    albedo = albedo * (1.f - skin.r) + float3(1.f, 0.05f, 0.12f) * skin.r;
    albedo = albedo * (1.f - skin.g) + float3(0.67f, 0.f, 0.f) * skin.g;
    
    // mouse
    albedo = albedo * (1.f - mouth.a) + mouth.rgb * mouth.a;
    
    // innerlight
    float3 eye = normalize(mul(float4(input.vPosWorld, 1.f), g_matView)).xyz;
    float3 viewNormal = normalize(mul(float4(input.vNormalWorld, 0.f), g_matView)).xyz;
    float brightAlpha = 1.f - pow(saturate(abs(dot(-eye, viewNormal))), 0.1f);
    brightAlpha = brightAlpha * 0.8f + 0.1f; // 최소값: 0.1f, 최대값: 0.9f
    albedo = albedo + float3(1.f, 0.87f, 0.93f) * brightAlpha;
    
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
        normalWorld = normalize(input.vNormalWorld);
        float3 normal = eyeNormal.rgb;
        normal.b = 1.f;
        normal = 2.0 * normal - 1.0;
        
        float3 N = normalWorld;
        float3 T = normalize(input.vTangentWorld - dot(input.vTangentWorld, N) * N);
        float3 B = normalize(cross(N, T));
        
        float3x3 TBN = float3x3(T, B, N);
        normalWorld = float4(normalize(mul(normal, TBN)), 1.f);
    }
    else
    {
        normalWorld = float4(normalize(input.vNormalWorld), 1.f);
    }

    float metallic = MtrlMetallic;
    float roughness = MtrlRoughness;
    float ao = SSAOTex.Sample(g_LinearWrapSampler, input.vUV0).r;
    float3 emission = MtrlEmission.rgb;
    
    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        DirectLighting(i, input.vPosWorld, normalWorld, albedo, ao, metallic, roughness, directLighting);
    }
    
    // Lightbulb 광원 연산
    float3 InnerLighting = (float3) 0.f;
    for (uint i = 0; i < g_LightCount; ++i)
    {
        tLightInfo LightInfo = g_Light[i];
        if (LIGHT_POINT == LightInfo.LightType)
        {
            float fDot = dot(normalWorld, pixelToEye);
            fDot = saturate(smoothstep(0.0, 1.0, fDot)); // saturate() 0 ~ 1 Climp 
            
            float3 lightVec = LightInfo.vWorldPos - input.vPosWorld;
            float lightDist = length(lightVec);
            lightVec /= lightDist;
            
            // Distance attenuation
            float att = saturate((LightInfo.fallOffEnd - lightDist)
                         / (LightInfo.fallOffEnd - LightInfo.fallOffStart));
    
            InnerLighting += albedo * LightInfo.vRadiance.rgb * fDot * att;
        }
    }
    
    PS_OUT_FORWARD output;
    output.vColor = float4(ambientLighting + directLighting + InnerLighting + emission, 1.0);
    output.vColor = clamp(output.vColor, 0.0, 1000.0);

    output.vMotionVector.xy = input.vMotionVector.xy; // Vector
    output.vMotionVector.z = 1.f;
    output.vMotionVector.w = input.vMotionVector.z / input.vMotionVector.w; // Depth
    
    return output;
}