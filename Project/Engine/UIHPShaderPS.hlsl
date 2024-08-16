
#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN _in) : SV_Target
{
    float4 output = _in.vUV0.x >= g_float_0 ? g_vec4_1 : g_vec4_0;
 
    if (_in.vUV0.x >= g_float_1)
    {
        output = float4(0.f, 0.f, 0.f, 0.f);
    }

    // Texture Exist
    if (g_btex_0)
    {
        float4 texColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
        
        output = texColor.r >= 0.5 ? texColor.a : output;
        
        if (texColor.a <= 0.1f)
            discard;
    }
    
    return output;
}