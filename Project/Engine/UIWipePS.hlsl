#include "struct.hlsli"
#include "global.hlsli"

#define POSTTEX g_postprocess_Tex
#define MASKINGPOSTTEX g_tex_0
#define BACKGROUNDCOLOR g_vec4_0

float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    float4 MaskingColor = (float4) 0.f;
    
    MaskingColor = MASKINGPOSTTEX.Sample(g_LinearWrapSampler, _in.vUV0);
    output = (MaskingColor.r < 0.95f || MaskingColor.g < 0.95f || MaskingColor.b < 0.95f) ? BACKGROUNDCOLOR : POSTTEX.Sample(g_LinearWrapSampler, _in.vUV0);
    output.a = 1.f;
    
    return output;
}