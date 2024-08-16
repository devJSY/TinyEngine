
#include "struct.hlsli"
#include "global.hlsli"

#define ALPHADISCARD g_float_0
#define USECOLORTEX g_int_0

#define STATIC_COLOR g_vec4_0

#define ShapeTex g_tex_0
#define DittherTex g_tex_1
#define ColorTex g_tex_2

float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
    // Shaping Texture Exist
    if (g_btex_0)
    {
        float stencilColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0).a;
        if (stencilColor >= 0.1f)
        {
            if (USECOLORTEX)
            {
                if (g_btex_1)
                {
                    output = DittherTex.Sample(g_LinearWrapSampler, _in.vUV0);
                    if (output.g <= g_float_0)
                    {
                        output = float4(127.f / 255.f, 127.f / 255.f, 127.f / 255.f, 1.f);
                    }
                    else
                    {
                        discard;
                    }
                }
                else
                {
                    output = ColorTex.Sample(g_LinearWrapSampler, _in.vUV0);
                }
            }
            else
            {
                output = STATIC_COLOR;
            }
        }
        else
        {
            discard;
        }
    }

    output.a = 1.f;
    
    return output;
}