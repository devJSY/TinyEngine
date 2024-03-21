#include "global.hlsli"
#include "struct.hlsli"

#define MaxValue g_float_0
#define CurValue g_float_1

#define RenderColor g_vec4_0

float4 main(PS_IN _in) : SV_Target
{
    float CurProgress = CurValue / MaxValue;
    if (CurProgress < _in.vUV.x)
        discard;
    
    float4 vColor = float4(0.0, 0.0, 0.0, 1.0);

    if (g_btex_0)
        vColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
  
    if (0.1f >= vColor.a)
        discard;
    
    return RenderColor;
}
