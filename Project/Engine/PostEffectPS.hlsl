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

float4 main(PS_IN input) : SV_TARGET
{
    if (0 == MODE)  // 0: Rendered image
    {
        float fogMin = 100.0;
        float fogMax = 1000.0;
        
        float4 posView = TexcoordToView(input.vUV);
        
        float dist = length(posView.xyz);
        float distFog = saturate((dist - fogMin) / (fogMax - fogMin));
        float fogFactor = exp(-distFog * FogStrength);
        
        float3 color = RenderTex.Sample(g_LinearClampSampler, input.vUV).rgb;
        
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
