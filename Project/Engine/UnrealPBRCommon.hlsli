#ifndef _UNREALPBRCOMMON
#define _UNREALPBRCOMMON

#include "struct.hlsli"
#include "global.hlsli"
#include "disksample.hlsli"

#define BRDFTex g_BRDFTex // SpecularIBL Look-up Table
#define SpecularIBLTex g_SpecularCube
#define IrradianceIBLTex g_DiffuseCube 

static const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
static float LightRadiusScale = 1e-2f;

// 보는 각도에 따라서 색이나 밝기가 달라 짐
float3 SchlickFresnel(float3 F0, float NdotH)
{
    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
    //return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0); 
}

float3 DiffuseIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                  float metallic)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(normalWorld, pixelToEye)));
    float3 kd = lerp(1.0 - F, 0.0, metallic); // metallic가 커질수록 Diffuse가 감소됨 - 에너지 보존 법칙
    float3 irradiance = IrradianceIBLTex.SampleLevel(g_LinearWrapSampler, normalWorld, 0).rgb;
    
    return kd * albedo * irradiance;
}

float3 SpecularIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                   float metallic, float roughness)
{
    float IBLRange = 5.f; // SpecularIBLTex.GetDimensions() Lod
    float IBLBias = 0.f;
    float lod = roughness * IBLRange + IBLBias;

    float2 specularBRDF = BRDFTex.SampleLevel(g_LinearClampSampler, float2(dot(normalWorld, pixelToEye), 1.0 - roughness), 0.0f).rg;
    float3 specularIrradiance = SpecularIBLTex.SampleLevel(g_LinearWrapSampler, reflect(-pixelToEye, normalWorld), lod).rgb;

    float3 F0 = lerp(Fdielectric, albedo, metallic); // metallic가 커질수록 albedo값에 가까워짐 - 에너지 보존 법칙

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 AmbientLightingByIBL(float3 albedo, float3 normalW, float3 pixelToEye, float ao,
                            float metallic, float roughness)
{
    float3 diffuseIBL = DiffuseIBL(albedo, normalW, pixelToEye, metallic);
    float3 specularIBL = SpecularIBL(albedo, normalW, pixelToEye, metallic, roughness);
    
    return (diffuseIBL + specularIBL) * ao;
}

// GGX/Towbridge-Reitz normal distribution function. // 미세 표면중 보는 방향이 노말인 표면의 비율
// Uses Disney's reparametrization of alpha = roughness^2.
float NdfGGX(float NdotH, float roughness, float alphaPrime)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;

    return alphaPrime * alphaPrime / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float SchlickG1(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
// 미세표면중 울퉁불퉁한 부분의 경우 빛이 들어오더라도 나가다가 막히는 Masking 을 표현하는 기하학적인 형태의 함수
float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return SchlickG1(NdotI, k) * SchlickG1(NdotO, k);
}

#define LIGHT_NEAR_PLANE 0.1f
#define LIGHT_FRUSTUM_WIDTH 0.2f // Near 0.1f Far 10000.f FOV 90° 기준 
#define LIGHT_TEXTURE_WIDTH 4096.f

// NdcDepthToViewDepth
float N2V(float ndcDepth, matrix invProj)
{
    float4 pointView = mul(float4(0, 0, ndcDepth, 1), invProj);
    return pointView.z / pointView.w;
}

float PCF_Filter(float2 uv, float zReceiverNdc, float filterRadiusUV, Texture2D shadowMap)
{
    float sum = 0.0f;
    for (int i = 0; i < 16; ++i)
    {
        float2 offset = diskSamples16[i] * filterRadiusUV;
        // SampleCmpLevelZero() - 밉맵 0 텍스춰에서 샘플링해온 첫번째 값과 인자값을 비교한다.
        // 주변 4개의 픽셀값 중 몇개가 조건에 만족하는지 비율로 결과값을 0 ~ 1 범위로 리턴한다.
        sum += shadowMap.SampleCmpLevelZero(
            g_ShadowCompareSampler, uv + offset, zReceiverNdc);
    }
    
    return sum / 16;
}

void FindBlocker(out float avgBlockerDepthView, out float numBlockers, float2 uv,
                 float zReceiverView, Texture2D shadowMap, matrix invProj, float lightRadiusWorld)
{
    float lightRadiusUV = lightRadiusWorld / LIGHT_FRUSTUM_WIDTH;
    
    // 광원 Near Plane 위에서의 탐색 범위
    // UV 좌표 범위
    float searchRadius = lightRadiusUV * (zReceiverView - LIGHT_NEAR_PLANE) / zReceiverView;

    float blockerSum = 0;
    numBlockers = 0;
    for (int i = 0; i < 16; ++i)
    {
        float shadowMapDepth =
            shadowMap.SampleLevel(g_ShadowPointSampler, float2(uv + diskSamples16[i] * searchRadius), 0).r;

        shadowMapDepth = N2V(shadowMapDepth, invProj);
        
        if (shadowMapDepth < zReceiverView)
        {
            blockerSum += shadowMapDepth;
            numBlockers++;
        }
    }
    
    avgBlockerDepthView = blockerSum / numBlockers;
}

float PCSS(float2 uv, float zReceiverNdc, Texture2D shadowMap, matrix invProj, float lightRadiusWorld)
{
    float zReceiverView = N2V(zReceiverNdc, invProj); // Light 관점에서 View 좌표계에서의 Z값
    
    // STEP 1: blocker search
    // 그림자가 지는 영역인지 아닌지 판별
    float avgBlockerDepthView = 0;
    float numBlockers = 0;

    FindBlocker(avgBlockerDepthView, numBlockers, uv, zReceiverView, shadowMap, invProj, lightRadiusWorld);

    if (numBlockers < 1)
    {
        // 그림자가 지지않는 영역이다
        return 1.f;
    }
    else
    {
        // STEP 2: penumbra size
        float penumbraRatio = (zReceiverView - avgBlockerDepthView) / avgBlockerDepthView; // FindBlocker에서 찾은 penumbra의 영역
        float lightRadiusUV = lightRadiusWorld / LIGHT_FRUSTUM_WIDTH; // Near Plane 위에서의 Light의 반지름
        float filterRadiusUV = penumbraRatio * lightRadiusUV * LIGHT_NEAR_PLANE / zReceiverView;

        // STEP 3: filtering
        return PCF_Filter(uv, zReceiverNdc, filterRadiusUV, shadowMap);
    }
}

float3 LightRadiance(tLightInfo light, float3 representativePoint, float3 posWorld, float3 normalWorld)
{
    // Default Point Light
    
    // Directional light
    float3 lightVec = light.LightType == LIGHT_DIRECTIONAL
                      ? -light.vWorldDir
                      : representativePoint - posWorld;

    float lightDist = length(lightVec);
    lightVec /= lightDist;

    // Spot light
    float spotFator = light.LightType == LIGHT_SPOT
                      ? pow(max(-dot(lightVec, light.vWorldDir), 0.0f), light.spotPower)
                      : 1.0f;
    
    // Distance attenuation
    float att = saturate((light.fallOffEnd - lightDist)
                         / (light.fallOffEnd - light.fallOffStart));
    
    // Shadow map
    float shadowFactor = 1.0;

    if (0 <= light.ShadowIndex)
    {
        // Project posWorld to light screen
        // light.viewProj 사용
        float4 pos = float4(posWorld, 1.0);
        float4 lightScreen = mul(pos, light.viewMat);
        lightScreen = mul(lightScreen, light.projMat);
        lightScreen.xyz /= lightScreen.w;
        
        // 카메라(광원)에서 볼 때의 텍스춰 좌표 계산
        // [-1, 1]x[-1, 1] -> [0, 1]x[0, 1]
        float2 lightTexcoord = float2(lightScreen.x, -lightScreen.y); // 텍스춰 좌표와 NDC는 y가 반대
        lightTexcoord += 1.0;
        lightTexcoord *= 0.5;
        
        // PCSS
        float bias = 0.001f;
        
        if (0 == light.ShadowIndex) // Dynamic Shadow
        {
            if (light.LightType == LIGHT_DIRECTIONAL)
                shadowFactor = PCF_Filter(lightTexcoord, lightScreen.z - bias, light.fRadius / LIGHT_TEXTURE_WIDTH, g_DynamicLightDepthMapTex1);
            else
                shadowFactor = PCSS(lightTexcoord, lightScreen.z - bias, g_DynamicLightDepthMapTex1, light.invProj, light.fRadius * LightRadiusScale);
        }
        // Static Shadow
        else if (1 == light.ShadowIndex)
        {
            if (light.LightType == LIGHT_DIRECTIONAL)
                shadowFactor = PCF_Filter(lightTexcoord, lightScreen.z - bias, light.fRadius / LIGHT_TEXTURE_WIDTH, g_StaticLightDepthMapTex1);
            else
                shadowFactor = PCSS(lightTexcoord, lightScreen.z - bias, g_StaticLightDepthMapTex1, light.invProj, light.fRadius * LightRadiusScale);
        }
        else if (2 == light.ShadowIndex)
        {
            if (light.LightType == LIGHT_DIRECTIONAL)
                shadowFactor = PCF_Filter(lightTexcoord, lightScreen.z - bias, light.fRadius / LIGHT_TEXTURE_WIDTH, g_StaticLightDepthMapTex2);
            else
                shadowFactor = PCSS(lightTexcoord, lightScreen.z - bias, g_StaticLightDepthMapTex2, light.invProj, light.fRadius * LightRadiusScale);
        }
        else if (3 == light.ShadowIndex)
        {
            if (light.LightType == LIGHT_DIRECTIONAL)
                shadowFactor = PCF_Filter(lightTexcoord, lightScreen.z - bias, light.fRadius / LIGHT_TEXTURE_WIDTH, g_StaticLightDepthMapTex3);
            else
                shadowFactor = PCSS(lightTexcoord, lightScreen.z - bias, g_StaticLightDepthMapTex3, light.invProj, light.fRadius * LightRadiusScale);
        }
    }

    float3 radiance = light.vRadiance.rgb * spotFator * att * shadowFactor;

    return radiance;
}

void DirectLighting(int _LightIdx, float3 _WorldPos, float3 _NormalWorld, float3 _Albedo, float _AO, float _Matallic, float _Roughness, inout float3 OutDirectLighting)
{
    tLightInfo LightInfo = g_Light[_LightIdx];
    
    const float3 pixelToEye = normalize(g_eyeWorld - _WorldPos);
    
    float3 lightVec = float3(0.f, 0.f, 0.f);
    float3 representativePoint = float3(0.f, 0.f, 0.f);
        
    if (LIGHT_DIRECTIONAL == LightInfo.LightType)
    {
        lightVec = -LightInfo.vWorldDir;
        representativePoint = LightInfo.vWorldPos;
    }
    else if (LIGHT_POINT == LightInfo.LightType)
    {
        // SphereLight 
        float3 L = LightInfo.vWorldPos - _WorldPos;
        float3 r = normalize(reflect(g_eyeWorld - _WorldPos, _NormalWorld));
        float3 centerToRay = dot(L, r) * r - L;
        float LightRadius = LightInfo.fRadius * LightRadiusScale;
            
        representativePoint = L + centerToRay * clamp(LightRadius / length(centerToRay), 0.0, 1.0);
        representativePoint += _WorldPos;
        lightVec = representativePoint - _WorldPos;
    }
    else if (LIGHT_SPOT == LightInfo.LightType)
    {
        lightVec = LightInfo.vWorldPos - _WorldPos;
        representativePoint = LightInfo.vWorldPos;
    }
        
    float lightDist = length(lightVec);
    lightVec /= lightDist;
    float3 halfway = normalize(pixelToEye + lightVec);
        
    float NdotI = max(0.0, dot(_NormalWorld, lightVec));
    float NdotH = max(0.0, dot(_NormalWorld, halfway));
    float NdotO = max(0.0, dot(_NormalWorld, pixelToEye));

    float3 F0 = lerp(Fdielectric, _Albedo, _Matallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, pixelToEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), _Matallic);
    float3 diffuseBRDF = kd * _Albedo;

    // Sphere Normalization
    float alpha = _Roughness * _Roughness;
    float alphaPrime = _Roughness;
    if (LIGHT_POINT == LightInfo.LightType) // Point Light의 경우에만 Sphere Light 적용
    {
        alphaPrime = saturate(alpha + LightInfo.fRadius / (2.0 * lightDist));
    }
                
    float D = NdfGGX(NdotH, _Roughness, alphaPrime);
    float3 G = SchlickGGX(NdotI, NdotO, _Roughness);
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

    float3 radiance = LightRadiance(LightInfo, representativePoint, _WorldPos, _NormalWorld);
            
    OutDirectLighting += (diffuseBRDF + specularBRDF) * radiance * NdotI;
}

#endif