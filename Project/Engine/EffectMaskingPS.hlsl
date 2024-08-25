
#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MtrlAlbedo g_vAlbedo

float4 main(PS_IN _in) : SV_Target
{
    float4 vColor = float4(0.0, 0.0, 0.0, 1.0);
    
    float4 MaskingColor = g_tex_0.Sample(g_LinearClampSampler, _in.vUV0);
    
    if (MaskingColor.a <= 0.1f)
        discard;
    
    vColor = MtrlAlbedo;
    
    vColor.a = 1.f;
    
    return vColor;
}