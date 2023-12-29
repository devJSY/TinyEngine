#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float4 color = float4(0.0, 0.0, 0.0, 1.0);
    float4 texColor = g_Missing_tex.Sample(g_LinearSampler, input.vUV);
 
    [unroll] // warning X3550: sampler array index must be a literal expression, forcing loop to unroll
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        if (Lights[i].LightType & LIGHT_DIRECTIONAL)
            color.rgb += ComputeDirectionalLight(Lights[i], input.normalWorld, toEye);
        
        if (Lights[i].LightType & LIGHT_POINT)
            color.rgb += ComputePointLight(Lights[i], input.vPosWorld, input.normalWorld, toEye);
  
        if (Lights[i].LightType & LIGHT_SPOT)
            color.rgb += ComputeSpotLight(Lights[i], input.vPosWorld, input.normalWorld, toEye);
    }

    if (UseRim)
        color.rgb += RimLight(input.normalWorld, toEye, g_rimColor, g_rimPower);
    
    if (g_UseTexture)
    {
        int width = 0;
        int height = 0;
        int numMips = 0;
        
        // 2중 예외처리
        // 바인딩된 텍스춰가 실제로 존재할 경우에만 샘플링 
        g_tex_0.GetDimensions(0, width, height, numMips);
        if (!(0 == width || 0 == height))
            texColor = g_tex_0.Sample(g_LinearSampler, input.vUV);
        
        color = color * texColor;
    }
     
    return color;
}
