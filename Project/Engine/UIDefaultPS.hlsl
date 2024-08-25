
#include "struct.hlsli"
#include "global.hlsli"

#define ALPHADISCARD g_float_0
#define UIANIM1 g_int_0
#define FADEINOUT g_int_3

#define FLOAT_VALUE1 g_float_1
#define FLOAT_VALUE2 g_float_2
#define FLOAT_VALUE3 g_float_3


float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
    // Texture Exist
    if (g_btex_0)
    {
        output = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
          
        if (output.a <= ALPHADISCARD)
            discard;
    }
    
    // Glow Move Anim
    if (UIANIM1)
    {
        if (FLOAT_VALUE2 <= _in.vUV0.x && _in.vUV0.x <= FLOAT_VALUE3)
            output *= FLOAT_VALUE1;
    }
    
    if (FADEINOUT)
    {
        output.a = g_float_1;
    }
    else
    {
        output.a = 1.f;
    }
    
    if (_in.vUV0.x >= 0.99f)
        discard;
   
    return output;
}