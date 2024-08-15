
#include "struct.hlsli"
#include "global.hlsli"

#define ALPHADISCARD g_float_0
#define PatternTexture1 g_tex_0
#define PatternTexture2 g_tex_1

#define PatternUV1 g_vec2_0
#define PatternUV2 g_vec2_1

#define BACKGROUNDCOLOR g_vec4_0

float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
    float4 patternStencil1 = g_btex_0 ? g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0 + PatternUV1) : float4(0.f, 0.f, 0.f, 0.f);
    float4 patternStencil2 = g_btex_0 ? g_tex_1.Sample(g_LinearWrapSampler, _in.vUV0 + PatternUV2) : float4(0.f, 0.f, 0.f, 0.f);

    output = (patternStencil1.r >= g_float_0 && patternStencil2.r >= g_float_0) ? BACKGROUNDCOLOR : float4(1.f, 1.f, 1.f, 0.5f);
        
    return output;
}