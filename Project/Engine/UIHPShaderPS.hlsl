
#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define EmissionColor g_vec4_3

float4 main(PS_IN _in) : SV_Target
{
    if ((_in.vUV0.x <= 0.001f) && (_in.vUV0.y <= 0.5f) || (_in.vUV0.x >= 0.999f && _in.vUV0.y >= 0.5f))
    {
        discard;
    }
    
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
    
    output *= EmissionColor;
    
    return output;
}