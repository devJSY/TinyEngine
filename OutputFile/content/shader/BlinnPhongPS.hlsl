#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float4 color = float4(0.0, 0.0, 0.0, 1.0);

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

    // Rim
    if (UseRim)
        color.rgb += RimLight(input.normalWorld, toEye, g_rimColor, g_rimPower);
    
    // Texture
    if (g_UseTexture)
        color = color * g_tex_0.Sample(g_LinearSampler, input.vUV);
    
    // IBL
    float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
    float4 specular = float4(0.0, 0.0, 0.0, 0.0);
        
    if (g_btexcube_0)
    {
        diffuse = g_texCube_0.Sample(g_LinearSampler, input.normalWorld);
        diffuse.xyz *= g_vDiff.xyz;
    }

    if (g_btexcube_1)
    {
        specular = g_texCube_1.Sample(g_LinearSampler, reflect(-toEye, input.normalWorld));
        specular.xyz *= g_vSpec.xyz;
    }

    return color + diffuse + specular;
}
