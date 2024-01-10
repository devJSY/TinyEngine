#ifndef _STD2D
#define _STD2D

#include "global.hlsli"
#include "struct.hlsli"
#include "Light.hlsli"

struct VS_Input
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    float2 vUV : TEXCOORD;
};

struct VS_Output
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VS_Output VS_Std2D(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.0;
    
    output.vPosition = mul(float4(_in.vPos, 1.0), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.0), g_matWorld);
    
    return output;
}

float4 PS_Std2D(VS_Output _in) : SV_Target
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
            vColor = g_anim2d_tex.Sample(g_LinearSampler, vUV);
        }
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_LinearSampler, _in.vUV);
            //vColor = g_Data[1];
            
            //saturate 0 ~ 1 을 넘지 않게 보정
            float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
            if (fAlpha < 0.1f)
            {
                discard; //clip(-1);            
            }
        }
    }
    
    if (vColor.a < 0.1f)
        discard;
            
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    return vColor;
}

#endif
