
#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MtrlAlbedo g_vAlbedo

float4 main(PS_IN _in) : SV_Target
{
    float4 vColor = float4(0.0, 0.0, 0.0, 1.0);
    vColor = float4(MtrlAlbedo.rgb, 1.0);
    
    if (g_UseAnim2D)
    {
        float2 vUV = float2(0.0, 0.0);
        
        if (g_UseBackGround)
        {
            float2 vBackgroundLeftTop = g_vLeftTop + (g_vSliceSize / 2.f) - (g_vBackGround / 2.f);
            vBackgroundLeftTop -= g_vOffset;
            vUV = vBackgroundLeftTop + (g_vBackGround * _in.vUV0);
        }
        else
        {
            float2 LT = g_vLeftTop - g_vOffset;
            vUV = LT + (g_vSliceSize * _in.vUV0);
        }
        
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSliceSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSliceSize.y) < vUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV);
        }
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV0);
        }
        else
        {
            vColor = g_vAlbedo;
        }
    }
    
    if (0.1f >= vColor.a)
        discard;
    
    if (vColor.r >= 0.1f)
    {
        vColor = g_vAlbedo;
    }
    
    vColor.a = 1.f;
    
    return vColor;
}