#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "global.hlsli"

// mesh : RectMesh
struct VS_Input
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Output
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_Output VS_Postprocess(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

VS_Output VS_Postprocess_World(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

// ==========
// GrayFilter
// ==========
float4 PS_GrayFilter(VS_Output _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_postprocess_Tex.Sample(g_LinearClampSampler, _in.vUV);
    
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}

#define BlendColor g_vec4_0

float4 PS_BlendFilter(VS_Output _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_postprocess_Tex.Sample(g_LinearClampSampler, _in.vUV);
    
    float Alpha = g_vec4_0.a;

    float3 color = (vColor.rgb * (1.f - Alpha)) + (BlendColor.rgb * Alpha);
    
    return float4(color, 1.f);
}

// ==========
// Distortion
// ==========
float4 PS_Distortion(VS_Output _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // 픽셀쉐이더에 SV_Position 으로 입력된 값은 픽셀 쉐이더를 호출한 해당 픽셀의 좌표가 들어있다.
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    //vScreenUV.y += cos((vScreenUV.x + (g_time * (속도) )) * (주파수)) * (진폭);
    //vScreenUV.y += cos((vScreenUV.x + (g_time *  0.1f))   *   40.f)  *  0.1f;
    
    if (g_btex_0)
    {
        float2 vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_LinearWrapSampler, vUV).rg;
        vNoise = (vNoise.xy - 0.5f) * 0.1f;
        
        vScreenUV += vNoise;
    }
        
    vColor = g_postprocess_Tex.Sample(g_LinearClampSampler, vScreenUV);
    
    return vColor;
}

#define CinematicEnable g_int_0
#define Thickness g_float_0
#define CinematicColor g_vec4_0

float4 PS_CinematicFilter(VS_Output _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
        
    vColor = g_postprocess_Tex.Sample(g_LinearClampSampler, _in.vUV);
    
    if (CinematicEnable)
    {
        if ((_in.vUV.y >= 0.f && _in.vUV.y <= Thickness) || (_in.vUV.y >= 1.f - Thickness && _in.vUV.y <= 1.f))
        {
            vColor = g_vec4_0;
        }
    }
    
    return vColor;
}

float4 PS_AnimationFilter(VS_Output _in) : SV_Target
{
    float4 vColor = float4(0.0, 0.0, 0.0, 1.0);
    
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
#endif