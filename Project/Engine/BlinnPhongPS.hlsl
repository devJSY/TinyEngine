#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"

float4 main(VS_OUT input) : SV_TARGET
{
    float3 toEye = normalize(g_eyeWorld.xyz - input.vPosWorld);

    float3 color = float3(0.0, 0.0, 0.0);
 
    // 설정한 Ligth 외의 Light는 Strength 를 0 으로 설정 
    color += ComputeDirectionalLight(g_DirLight, input.normalWorld, toEye);
    color += ComputePointLight(g_PointLight, input.vPosWorld, input.normalWorld, toEye);
    color += ComputeSpotLight(g_SpotLight, input.vPosWorld, input.normalWorld, toEye);

    return g_UseTexture ? float4(color, 1.0) * g_AlbedoTex.Sample(g_LinearSampler, input.vUV) : float4(color, 1.0);
}
