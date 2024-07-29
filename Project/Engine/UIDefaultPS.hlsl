
#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    // Texture Exist
    if (g_btex_0)
    {
        output = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
        if (output.a <= 0.0001f)
            discard;
    }
    
    output.a = 1.f;
    
    return output;
}