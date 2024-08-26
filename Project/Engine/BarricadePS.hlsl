#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define Texture0 g_tex_0
#define Texture1 g_tex_1
#define Speed g_float_0
#define Alpha g_float_1
#define UVScale g_vec2_0
#define Color g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float4 vColor0 = (float4) 0.f;
    float4 vColor1 = (float4) 0.f;
    
    input.vUV0 *= UVScale;

    if (g_btex_0)
    {
        vColor0 = Texture0.Sample(g_LinearWrapSampler, input.vUV0 + float2(0.f, g_Time * Speed));
        vColor0.rgb *= vColor0.a;

    }
    if (g_btex_1)
    {
        vColor1 = Texture1.Sample(g_LinearWrapSampler, input.vUV0 + float2(0.f, g_Time * Speed));
        vColor1.rgb *= vColor1.a;
    }
    
    vOutColor.rgb = Color.rgb;
    vOutColor.rgb += vColor0.rgb;
    vOutColor.rgb += vColor1.rgb;
    vOutColor.a = Alpha;
    
    return vOutColor;
}

