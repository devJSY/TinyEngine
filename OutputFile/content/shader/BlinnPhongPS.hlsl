#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"

float4 main(VS_OUT input) : SV_TARGET
{
    float3 toEye = normalize(g_eyeWorld.xyz - input.vPosWorld);

    float3 color = float3(0.0, 0.0, 0.0);
 
    [unroll] // warning X3550: sampler array index must be a literal expression, forcing loop to unroll
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        if (Lights[i].LightType & LIGHT_DIRECTIONAL)
            color += ComputeDirectionalLight(Lights[i], input.normalWorld, toEye);
        
        if (Lights[i].LightType & LIGHT_POINT)
            color += ComputePointLight(Lights[i], input.vPosWorld, input.normalWorld, toEye);
  
        if (Lights[i].LightType & LIGHT_SPOT)
            color += ComputeSpotLight(Lights[i], input.vPosWorld, input.normalWorld, toEye);
    }   
   
    return g_UseTexture ? float4(color, 1.0) * g_AlbedoTex.Sample(g_LinearSampler, input.vUV) : float4(color, 1.0);
}
