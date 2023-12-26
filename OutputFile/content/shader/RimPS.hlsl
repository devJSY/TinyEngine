#include "struct.hlsli"
#include "global.hlsli"

float4 main(VS_OUT input) : SV_TARGET
{
    float3 toEye = normalize(g_eyeWorld.xyz - input.vPosWorld);

    float3 color = float3(0.0, 0.0, 0.0);
    float rim = 1.0 - dot(input.normalWorld, toEye);    

    rim = smoothstep(0.0, 1.0, rim);
    rim = pow(abs(rim), g_rimPower);
    
    color += rim * g_rimColor * g_rimStrength;

    return (color.r / 255.0, color.g / 255.0, color.g / 255.0, 1.0);
}