#include "struct.hlsli"
#include "global.hlsli"

#define MODE g_int_0 // 0: Rendered image, 1: DepthOnly 

#define FogStrength g_float_0
#define DepthScale g_float_1

#define RenderTex g_tex_0
#define DepthOnlyTex g_tex_1

#define FogColor g_vec4_0

float4 TexcoordToView(float2 texcoord)
{
    float4 posProj;

    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
    posProj.xy = texcoord * 2.0 - 1.0;
    posProj.y *= -1; // 주의: y 방향을 뒤집어줘야 합니다.
    posProj.z = DepthOnlyTex.Sample(g_LinearClampSampler, texcoord).r;
    posProj.w = 1.0;

    // ProjectSpace -> ViewSpace
    float4 posView = mul(posProj, g_matProjInv);
    posView.xyz /= posView.w;
    
    return posView;
}

int RaySphereIntersection(in float3 start, in float3 dir, in float3 center, in float radius,
                            out float t1, out float t2)
{
    float3 p = start - center;
    float pdotv = dot(p, dir);
    float p2 = dot(p, p);
    float r2 = radius * radius;
    float m = pdotv * pdotv - (p2 - r2);
    
    if (m < 0.0)
    {
        t1 = 0;
        t2 = 0;
        return 0; 
    }
    else
    {
        m = sqrt(m);
        t1 = -pdotv - m;
        t2 = -pdotv + m;
        return 1;
    }
}

// "Foundations of Game Engine Development" by Eric Lengyel, V2 p319
float HaloEmission(tLightInfo light, float3 posView)
{
    // Halo
    float3 rayStart = float3(0.f, 0.f, 0.f); // View space
    float3 dir = normalize(posView - rayStart);

    float3 center = mul(float4(light.vWorldPos, 1.f), g_matView).xyz; // View 공간으로 변환

    float t1 = 0.f;
    float t2 = 0.f;
    // Halo 범위에 들어오면서 눈앞에 물체가 없는경우에만 적용
    if (RaySphereIntersection(rayStart, dir, center, light.HaloRadius, t1, t2) && t1 < posView.z && t1 >= 0.f) 
    {
        t2 = min(posView.z, t2); // 물체가 가로막고있는 경우에 범위 축소
            
        float p2 = dot(rayStart - center, rayStart - center);
        float pdotv = dot(rayStart - center, dir);
        float r2 = light.HaloRadius * light.HaloRadius;
        float invr2 = 1.f / r2;
        float haloEmission = (1 - p2 * invr2) * (t2 - t1)
                          - pdotv * invr2 * (t2 * t2 - t1 * t1)
                          - 1.f / (3.f * r2) * (t2 * t2 * t2 - t1 * t1 * t1);
            
        haloEmission /= (4 * light.HaloRadius / 3.0); // Normalize

        return haloEmission;
    }
    else
    {
        return 0.f;
    }
}

float4 main(PS_IN input) : SV_TARGET
{
    if (0 == MODE)  // 0: Rendered image
    {
        float3 color = RenderTex.Sample(g_LinearClampSampler, input.vUV).rgb;
        float4 posView = TexcoordToView(input.vUV);
        
        // Halo
        float3 HaloColor = float3(0.96, 0.94, 0.82);
        for (uint i = 0; i < g_Light3DCount; ++i)
        {
            color += HaloEmission(g_Light3D[i], posView.xyz) * HaloColor * g_Light3D[i].HaloStrength;
        }

        // Fog
        float fogMin = 100.0;
        float fogMax = 1000.0;
        float dist = length(posView.xyz);
        float distFog = saturate((dist - fogMin) / (fogMax - fogMin));
        float fogFactor = exp(-distFog * FogStrength);
        
        color = lerp(FogColor.rgb, color, fogFactor);

        return float4(color, 1.0);

    }
    else // DepthOnly Mode
    {
        float z = TexcoordToView(input.vUV).z * DepthScale;
        float FarZ = 10000.f;
        z /= FarZ;
        return float4(z, z, z, 1);
    }
    
    return float4(0.f, 0.f, 0.f, 1.f);

}
