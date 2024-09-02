#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define Red g_float_0
#define Green g_float_1
#define Blue g_float_2

#define InnerSize g_vec2_0

float4 main(PS_IN _in) : SV_TARGET
{
    float4 vColor = g_postprocess_Tex.Sample(g_LinearClampSampler, _in.vUV0);
    float rEnable = Red;
    float gEnable = Green;
    float bEnable = Blue;
        
    float OuterVig = 1.f;
    float InnerVig = InnerSize.x;
        
    InnerVig = (InnerSize.y - InnerVig);
            
    float2 Center = float2(0.5f, 0.5f);
    
    float dist = distance(Center, _in.vUV0) * 1.414213;
        
    float vig = clamp((OuterVig - dist) / (OuterVig - InnerVig), 0.f, 1.f);
        
    if (vig != 1.0)
    {
        vColor *= vig;
        vColor.r += (1.0 - vig) * rEnable;
        vColor.g += (1.0 - vig) * gEnable;
        vColor.b += (1.0 - vig) * bEnable;
    }
    
    return vColor;
}