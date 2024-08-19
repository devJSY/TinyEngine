#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define EmissiveNosieTex g_tex_0
#define AlphaNosieTex g_tex_1

#define MtrlAlbedo g_vAlbedo
#define MtrlEmission g_vEmission

float4 main(PS_IN _in) : SV_Target0
{
    float3 OutColor = MtrlAlbedo;
    float Alpha = 1.f;
    
    // emissive noise
    if (g_btex_0)
    {
        float2 MoveUV = _in.vUV0 + g_EngineTime * 5.f;
        float Emissive = EmissiveNosieTex.Sample(g_LinearWrapSampler, MoveUV).r;
        OutColor = OutColor * (1.f - Emissive) + MtrlEmission.rgb * Emissive;
    }
    
    return float4(OutColor, Alpha);
}
