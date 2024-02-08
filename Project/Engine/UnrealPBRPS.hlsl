#include "struct.hlsli"
#include "global.hlsli"

#define BRDFTex g_BRDFTex // SpecularIBL Look-up Table
#define SpecularIBLTex g_SpecularCube
#define IrradianceIBLTex g_DiffuseCube 

#define AmbientTex g_tex_0
#define AOTex g_tex_1
#define NormalTex g_tex_2
#define MetallicRoughnessTex g_tex_4
#define EmissiveTex g_tex_5

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define InvertNormalMapY g_int_0

static const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0

float3 SchlickFresnel(float3 F0, float NdotH)
{
    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
    //return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0); 
}

float3 GetNormal(PS_IN input)
{
    float3 normalWorld = normalize(input.normalWorld);
    
    if (g_btex_2) // NormalWorld를 교체
    {
        float3 normal = NormalTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
        normal = 2.0 * normal - 1.0; // 범위 조절 [-1.0, 1.0]

        // OpenGL 용 노멀맵일 경우에는 y 방향을 뒤집어줍니다.
        normal.y = InvertNormalMapY ? -normal.y : normal.y;
        
        float3 N = normalWorld;
        float3 T = normalize(input.vTangentWorld - dot(input.vTangentWorld, N) * N);
        float3 B = cross(N, T);
        
        // matrix는 float4x4, 여기서는 벡터 변환용이라서 3x3 사용
        float3x3 TBN = float3x3(T, B, N);
        normalWorld = normalize(mul(normal, TBN));
    }
    
    return normalWorld;
}

float3 DiffuseIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                  float metallic)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(normalWorld, pixelToEye)));
    float3 kd = lerp(1.0 - F, 0.0, metallic);
    float3 irradiance = IrradianceIBLTex.SampleLevel(g_LinearWrapSampler, normalWorld, 0).rgb;
    
    return kd * albedo * irradiance;
}

float3 SpecularIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                   float metallic, float roughness)
{
    float IBLRange = 5.f;
    float IBLBias = 0.f;
    float lod = roughness * IBLRange + IBLBias;
    
    float2 specularBRDF = BRDFTex.SampleLevel(g_LinearClampSampler, float2(dot(normalWorld, pixelToEye), 1.0 - roughness), 0.0f).rg;
    float3 specularIrradiance = SpecularIBLTex.SampleLevel(g_LinearWrapSampler, reflect(-pixelToEye, normalWorld), lod).rgb;

    float3 F0 = lerp(Fdielectric, albedo, metallic);

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 AmbientLightingByIBL(float3 albedo, float3 normalW, float3 pixelToEye, float ao,
                            float metallic, float roughness)
{
    float3 diffuseIBL = DiffuseIBL(albedo, normalW, pixelToEye, metallic);
    float3 specularIBL = SpecularIBL(albedo, normalW, pixelToEye, metallic, roughness);
    
    return (diffuseIBL + specularIBL) * ao;
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float NdfGGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;

    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float SchlickG1(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return SchlickG1(NdotI, k) * SchlickG1(NdotO, k);
}

float3 LightRadiance(tLightInfo light, float3 posWorld, float3 normalWorld)
{
    // Default Point Light
    
    // Directional light
    float3 lightVec = light.LightType == LIGHT_DIRECTIONAL
                      ? -light.vWorldDir
                      : light.vWorldPos - posWorld;

    float lightDist = length(lightVec);
    lightVec /= lightDist;

    // Spot light
    float spotFator = light.LightType == LIGHT_SPOT
                      ? pow(max(-dot(lightVec, light.vWorldDir), 0.0f), light.spotPower)
                      : 1.0f;
        
    // Distance attenuation
    float att = saturate((light.fallOffEnd - lightDist)
                         / (light.fallOffEnd - light.fallOffStart));

    float3 radiance = light.vRadiance.rgb * spotFator * att;

    return radiance;
}

float4 main(PS_IN input) : SV_TARGET
{
    float3 pixelToEye = normalize(g_eyeWorld - input.vPosWorld);
    float3 normalWorld = GetNormal(input);
    
    float3 albedo = g_btex_0 ? AmbientTex.Sample(g_LinearWrapSampler, input.vUV).rgb 
                                 : MtrlAlbedo.rgb;
    float ao = g_btex_1 ? AOTex.Sample(g_LinearWrapSampler, input.vUV).r : 1.0;
    float metallic = g_btex_4 ? MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).b
                                    : MtrlMetallic;
    float roughness = g_btex_4 ? MetallicRoughnessTex.Sample(g_LinearWrapSampler, input.vUV).g 
                                      : MtrlRoughness;
    float3 emission = g_btex_5 ? EmissiveTex.Sample(g_LinearWrapSampler, input.vUV).rgb
                                     : MtrlEmission.rgb;

    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao, metallic, roughness);
    
    float3 directLighting = float3(0, 0, 0);

    for (uint i = 0; i < g_Light3DCount; ++i)
    {
        float3 lightVec = g_Light3D[i].LightType == LIGHT_DIRECTIONAL
                      ? -g_Light3D[i].vWorldDir
                      : g_Light3D[i].vWorldPos - input.vPosWorld;
        
        float lightDist = length(lightVec);
        lightVec /= lightDist;
        float3 halfway = normalize(pixelToEye + lightVec);
        
        float NdotI = max(0.0, dot(normalWorld, lightVec));
        float NdotH = max(0.0, dot(normalWorld, halfway));
        float NdotO = max(0.0, dot(normalWorld, pixelToEye));

        float3 F0 = lerp(Fdielectric, albedo, metallic);
        float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, pixelToEye)));
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
        float3 diffuseBRDF = kd * albedo;

        float D = NdfGGX(NdotH, roughness);
        float3 G = SchlickGGX(NdotI, NdotO, roughness);
        float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

        float3 radiance = LightRadiance(g_Light3D[i], input.vPosWorld, normalWorld);
            
        directLighting += (diffuseBRDF + specularBRDF) * radiance * NdotI;
    }
    
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    output = float4(ambientLighting + directLighting + emission, 1.0);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}
