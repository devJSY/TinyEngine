#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define Texture g_tex_0
#define Speed g_float_0
#define Color1 g_vec4_0
#define Color2 g_vec4_1

float4 main(PS_IN input) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    vOutColor.a = 1.f;

    if (g_btex_0)
    {
        if(input.vUV0.y > 0.5f)
        {
            discard;
        }
        
        float4 SampleColor = Texture.Sample(g_LinearWrapSampler, input.vUV0 + (float2(g_Time, sin(g_Time)) * Speed));
        vOutColor.rgb = lerp(Color1.rgb, Color2.rgb, input.vUV0.x);
        vOutColor.rgb *= SampleColor.rgb;
        vOutColor.a = SampleColor.a;
    }
    
    return vOutColor;
}

