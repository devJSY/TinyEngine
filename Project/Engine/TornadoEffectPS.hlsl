#include "struct.hlsli"
#include "global.hlsli"
#include "Light.hlsli"
#include "func.hlsli"

#define Texture g_tex_0

#define ALPHA g_float_0
#define TORNADO_COLOR g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    vOutColor.a = ALPHA;

    if (g_btex_0)
    {
        float Alpha = Texture.Sample(g_LinearWrapSampler, input.vUV0 - float2(g_Time, g_Time)).x;
        vOutColor.rgb = lerp(float3(1.f, 1.f, 1.f), TORNADO_COLOR.rgb, Alpha);
        vOutColor.a *= Alpha;
    }
    
    return vOutColor;
}

