
#include "struct.hlsli"
#include "global.hlsli"

float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
    if (_in.vUV.x >= g_float_0)
    {
        output = g_vec4_1;
    }
    else
    {
        output = g_vec4_0;
    }
    
    if (_in.vUV.x >= g_float_1)
    {
        output = float4(0.f, 0.f, 0.f, 0.f);
    }
    
    return output;
}