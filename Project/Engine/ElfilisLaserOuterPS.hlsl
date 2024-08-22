#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define BlackNosieTex g_tex_0
#define AlphaNosieTex g_tex_1
#define EmissiveNosieTex g_tex_2

#define MtrlAlbedo g_vAlbedo
#define MtrlEmission g_vEmission

float4 main(PS_IN _in) : SV_Target0
{
    float3 OutColor = MtrlEmission.rgb;
    float Alpha = 0.f;
    
    // ¿Ü°ûÃ³¸®
    float3 pixelToEye = normalize(g_eyeWorld - _in.vPosWorld);
    float3 normalWorld = _in.vNormalWorld;
    Alpha = 1.f - pow(saturate(abs(dot(-pixelToEye, normalWorld))), 0.1f);
    
    // emissive noise
    if (g_btex_0)
    {
        float2 MoveUV = _in.vUV0 * 2.f + g_EngineTime * 9.f;
        float Emissive = EmissiveNosieTex.Sample(g_LinearWrapSampler, MoveUV).r;
        
        if (Emissive > 0.1f)
        {
            Alpha = Emissive;
        }
    }
    
    // black noise
    if (g_btex_0)
    {
        float2 MoveUV = _in.vUV0 * 1.f + g_EngineTime * 9.f;
        float Black = BlackNosieTex.Sample(g_LinearWrapSampler, MoveUV).r;
        Black = pow(Black, 0.1f);
        
        if (Black > 0.9f)
        {
            OutColor = (float3) 0.f;
            Alpha = 1.f;
        }
    }
    
    if (Alpha <= 0.f)
        clip(-1);
    
    return float4(OutColor, Alpha);
}
