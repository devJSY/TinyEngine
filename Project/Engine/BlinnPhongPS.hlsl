#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"

#define SpecularIBLTex g_SpecularCube
#define IrradianceIBLTex g_DiffuseCube 

#define MtrlDiffuse g_vDiffuse
#define MtrlSpecular g_vSpecular
 
#define RIM_COLOR g_vec4_0
#define RIM_POWER g_float_0

float4 main(PS_IN input) : SV_Target
{
    float3 toEye = normalize(g_eyeWorld - input.vPosWorld);
    float4 color = float4(0.0, 0.0, 0.0, 1.0);

    for (uint i = 0; i < g_LightCount; ++i)
    {
        if (LIGHT_DIRECTIONAL == g_Light[i].LightType)
            color.rgb += ComputeDirectionalLight(g_Light[i], input.vNormalWorld, toEye);
        
        if (LIGHT_POINT == g_Light[i].LightType)
            color.rgb += ComputePointLight(g_Light[i], input.vPosWorld, input.vNormalWorld, toEye);
  
        if (LIGHT_SPOT == g_Light[i].LightType)
            color.rgb += ComputeSpotLight(g_Light[i], input.vPosWorld, input.vNormalWorld, toEye);
    }

    // Rim
    color.rgb += RimLight(input.vNormalWorld, toEye, RIM_COLOR.rgb, RIM_POWER);
    
    // Texture
    if (g_btex_0)
    {
        color = color * g_tex_0.Sample(g_LinearWrapSampler, input.vUV);
    }
    
    // IBL
    float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
    float4 specular = float4(0.0, 0.0, 0.0, 0.0);
    
    diffuse = IrradianceIBLTex.Sample(g_LinearWrapSampler, input.vNormalWorld);
    diffuse.xyz *= MtrlDiffuse.xyz;
    
    specular = SpecularIBLTex.Sample(g_LinearWrapSampler, reflect(-toEye, input.vNormalWorld));
    specular.xyz *= MtrlSpecular.xyz;

    return color + diffuse + specular;
}
