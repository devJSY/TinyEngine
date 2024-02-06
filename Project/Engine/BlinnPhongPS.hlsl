#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"

#define MtrlDiffuse g_vDiffuse
#define MtrlSpecular g_vSpecular

float4 main(PS_IN input) : SV_Target
{
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float4 color = float4(0.0, 0.0, 0.0, 1.0);

    for (uint i = 0; i < g_Light3DCount; ++i)
    {
        if (LIGHT_DIRECTIONAL == g_Light3D[i].LightType)
            color.rgb += ComputeDirectionalLight(g_Light3D[i], input.normalWorld, toEye);
        
        if (LIGHT_POINT == g_Light3D[i].LightType)
            color.rgb += ComputePointLight(g_Light3D[i], input.vPosWorld, input.normalWorld, toEye);
  
        if (LIGHT_SPOT == g_Light3D[i].LightType)
            color.rgb += ComputeSpotLight(g_Light3D[i], input.vPosWorld, input.normalWorld, toEye);
    }

    // Rim
    if (g_UseRim)
        color.rgb += RimLight(input.normalWorld, toEye, g_rimColor, g_rimPower);
    
    // Texture
    if (g_btex_0)
        color = color * g_tex_0.Sample(g_LinearWrapSampler, input.vUV);
    
    // IBL
    float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
    float4 specular = float4(0.0, 0.0, 0.0, 0.0);
        
    if (g_btexcube_0)
    {
        diffuse = g_texCube_0.Sample(g_LinearWrapSampler, input.normalWorld);
        diffuse.xyz *= MtrlDiffuse.xyz;
    }

    if (g_btexcube_1)
    {
        specular = g_texCube_1.Sample(g_LinearWrapSampler, reflect(-toEye, input.normalWorld));
        specular.xyz *= MtrlSpecular.xyz;
    }

    return color + diffuse + specular;
}
