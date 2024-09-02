#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MtrlAlbedo g_vAlbedo

#define Texture g_tex_0
#define Alpha g_float_2
#define UseChangeColor g_int_0
#define ChangeColor g_vec4_0

float4 main(PS_IN input) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    vOutColor.a = 1.f;

    // get UV
    float2 QuadUV = input.vUV0;
    
    if (QuadUV.x < 0.5)
    {
        QuadUV.x /= 0.5f;
    }
    else
    {
        QuadUV.x = 1.f - (QuadUV.x - 0.5f) / 0.5f;
    }
    
    if (QuadUV.y < 0.5)
    {
        QuadUV.y /= 0.5f;
    }
    else
    {
        QuadUV.y = 1.f - (QuadUV.y - 0.5f) / 0.5f;
    }
    
    // Color
    float4 Color = MtrlAlbedo;
    
    if (UseChangeColor)
    {
        float period = 1.f;
        float AccTime = g_EngineTime % period;
        float Ratio = AccTime / period;
        float Delta1 = (sin(PI * 2.f * Ratio + PI / 2.f) + 1.f) / 2.f; // 0~1·Î ¸ÅÇÎ
        float Delta2 = 1 - Delta1;
    
        Color = Color * Delta1 + ChangeColor * Delta2;
    }
    
    // Sampling Texture
    if (g_btex_0)
    {
        float4 SampleColor = Texture.Sample(g_LinearWrapSampler, QuadUV);
        vOutColor = Color * SampleColor;
    }
    
    vOutColor.a *= Alpha;
    return vOutColor;
}