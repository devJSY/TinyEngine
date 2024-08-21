#include "struct.hlsli"
#include "global.hlsli"

#define ALPHADISCARD g_float_0

float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
   // Texture Exist
    if (g_btex_0)
    {
        output = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
        
        //output *= output.a;
        
        if (output.a <= ALPHADISCARD)
            discard;
    }
    
    return output;
}