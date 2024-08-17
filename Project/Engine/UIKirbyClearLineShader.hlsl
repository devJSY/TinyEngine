
#include "struct.hlsli"
#include "global.hlsli"

#define ALPHADISCARD g_float_0

#define STATICCOLOR g_vec4_0
#define STATICCOLOR2 g_vec4_1


float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
    // Texture Exist
    if (g_btex_0)
    {
        output = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
        if (output.a <= 0.1f)
            discard;
    }
    
    output = output.r <= ALPHADISCARD ? STATICCOLOR : STATICCOLOR2;
   
    output.a = 1.f;
    
    return output;
}