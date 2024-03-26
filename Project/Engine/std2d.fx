#ifndef _STD2D
#define _STD2D

#include "global.hlsli"
#include "struct.hlsli"
#include "Light.hlsli"

#define MtrlAlbedo g_vAlbedo

PS_IN VS_Std2D(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.0;
    
    output.vPosProj = mul(float4(_in.vPos, 1.0), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vPosWorld = mul(float4(_in.vPos, 1.0), g_matWorld).xyz;
    
    return output;
}

float4 PS_Std2D(PS_IN _in) : SV_Target
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
            vUV = vBackgroundLeftTop + (g_vBackGround * _in.vUV);
        }
        else
        {
            float2 LT = g_vLeftTop - g_vOffset;
            vUV = LT + (g_vSliceSize * _in.vUV);
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
            vColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
        }
    }
  
    if (0.1f >= vColor.a)
        discard;
    
    vColor.a = 1.f;
    
    return vColor;
}

float4 PS_Std2D_Light(PS_IN _in) : SV_Target
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
            vUV = vBackgroundLeftTop + (g_vBackGround * _in.vUV);
        }
        else
        {
            float2 LT = g_vLeftTop - g_vOffset;
            vUV = LT + (g_vSliceSize * _in.vUV);
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
            vColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
        }
    }
    
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    tLightInfo LightColor = (tLightInfo) 0.f;
    
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vPosWorld, i, LightColor);
    }
    
    vColor.rgb *= LightColor.vRadiance.rgb;
    
    if (0.1f >= vColor.a)
        discard;
            
    vColor.a = 1.f;
    
    return vColor;
}

#endif

// 텍스춰의 Alpha값 사용
float4 PS_Std2D_Effect(PS_IN _in) : SV_Target
{
    float4 vColor = float4(0.0, 0.0, 0.0, 0.0);
    vColor = float4(MtrlAlbedo.rgb, 0.0);
    
    if (g_UseAnim2D)
    {
        float2 vUV = float2(0.0, 0.0);
        
        if (g_UseBackGround)
        {
            float2 vBackgroundLeftTop = g_vLeftTop + (g_vSliceSize / 2.f) - (g_vBackGround / 2.f);
            vBackgroundLeftTop -= g_vOffset;
            vUV = vBackgroundLeftTop + (g_vBackGround * _in.vUV);
        }
        else
        {
            float2 LT = g_vLeftTop - g_vOffset;
            vUV = LT + (g_vSliceSize * _in.vUV);
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
            vColor = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
        }
    }
    
    if (0.1f >= vColor.a)
        discard;
    
    vColor.rgb = MtrlAlbedo.rgb * vColor.a;
    vColor.a = 1.f;

    return vColor;
}
