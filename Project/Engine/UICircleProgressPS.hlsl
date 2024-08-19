
#include "struct.hlsli"
#include "global.hlsli"

#define PROGRESS g_float_0
#define STATICCOLOR g_vec4_0

#define TEST g_float_1
#define FADEINOUT g_int_3


float4 main(PS_IN _in) : SV_Target
{
    float4 output = (float4) 0.f;
    
    // Texture Exist
    if (g_btex_0)
    {
        output = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
        
        if (output.r >= 0.9f && output.g <= 0.1f)
        {
            float2 NewUV = float2(_in.vUV0.x - 0.5f, _in.vUV0.y * -1.f + 0.5f);
            float2 Progress = _in.vUV0 - float2(0.5, 0.5f);
            NewUV = normalize(NewUV);
            
            float2 StandDard = float2(0.f, -1.f);
            float2 ComparePoint = float2(_in.vUV0);
            
            float fRadian = dot(NewUV, StandDard);
           
            // https://gpgstudy.com/forum/viewtopic.php?t=408
            float PIRadian = (NewUV.y * StandDard.x + NewUV.x * StandDard.y > 0.0f) ? (float) (acos(fRadian)) : -(float) (acos(fRadian));
            
            float Degree = degrees(PIRadian);
            
            if (Degree <= PROGRESS - 180.f)
            {
                output.rgb = STATICCOLOR.rgb;
            }
            else
            {
                output.a = 0.f;
            }
        }
        
        if (output.a <= 0.1f)
            discard;
    }
   
    if (FADEINOUT)
    {
        output.a = g_float_1;
    }
    else
    {
        output.a = 1.f;
    }
   
    return output;
}