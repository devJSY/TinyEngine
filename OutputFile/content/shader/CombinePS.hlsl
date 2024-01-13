#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float3 postprocessTex = float3(0.f, 0.f, 0.f);
    float3 RTCopyTex = float3(0.f, 0.f, 0.f);
    
    postprocessTex = g_postprocess_Tex.Sample(g_PointSampler, input.vUV).rgb;
      
    if (g_btex_0)
        RTCopyTex = g_tex_0.Sample(g_PointSampler, input.vUV).rgb;
    
    return float4(postprocessTex + g_Bloom_Strength * RTCopyTex, 1.f);
}