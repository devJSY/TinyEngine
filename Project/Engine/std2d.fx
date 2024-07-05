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
    output.vUV0 = _in.vUV0;
    
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
            discard;
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
            discard;
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

#define GlowEnable g_int_0
#define GlowThreshold g_float_0
#define GlowColor g_vec4_0
#define UseAlbedo g_int_1

struct PS_Std2D_Glow_Output
{
    float4 RenderTargetColor : SV_Target0;
    float4 BloomTextureColor : SV_Target1;
};

// 텍스춰의 Alpha값 사용
PS_Std2D_Glow_Output PS_Std2D_Effect(PS_IN _in) : SV_Target
{
    PS_Std2D_Glow_Output output;
    
    float4 vColor = float4(0.0, 0.0, 0.0, 1.0);
    
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
            discard;
        }
    }
    
    // Relative Luminance : 픽셀의 색이 밝은지 어두운지의 기준값
    float RelativeLuminance = dot(vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
        
    // 일정 GlowThreshold 이상인 경우 지정된 색상으로 마스킹
    if (GlowEnable && RelativeLuminance > GlowThreshold)
    {
        output.BloomTextureColor = GlowColor;
    }
    else
    {
        output.BloomTextureColor = float4(0.f, 0.f, 0.f, 1.f);
    }
  
    if (UseAlbedo)
        vColor.rgb = MtrlAlbedo.rgb;
        
    output.RenderTargetColor = vColor;
    
    return output;
}

#define GlowEnable g_int_0
#define GlowThreshold g_float_0
#define GlowColor g_vec4_0

PS_Std2D_Glow_Output PS_Std2D_Glow(PS_IN _in)
{
    PS_Std2D_Glow_Output output;
    
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
            discard;
        }
    }
        
    if (0.1f >= vColor.a)
        discard;
    
    // Relative Luminance : 픽셀의 색이 밝은지 어두운지의 기준값
    float RelativeLuminance = dot(vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
        
    // 일정 GlowThreshold 이상인 경우 지정된 색상으로 마스킹
    if (GlowEnable && RelativeLuminance > GlowThreshold)
    {
        output.BloomTextureColor = GlowColor;
    }
    else
    {
        output.BloomTextureColor = float4(0.f, 0.f, 0.f, 1.f);
    }
  
    // 광원 처리
    tLightInfo LightColor = (tLightInfo) 0.f;
    
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vPosWorld, i, LightColor);
    }
    
    vColor.rgb *= LightColor.vRadiance.rgb;
    vColor.a = 1.f;
    
    output.RenderTargetColor = vColor;
    
    return output;
}

#endif
