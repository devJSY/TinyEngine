#ifndef _STD2D
#define _STD2D

#include "global.hlsli"
#include "struct.hlsli"

PS_IN VS_Std2D(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.0;
    
    output.vPosProj = mul(float4(_in.vPos, 1.0), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(PS_IN _in) : SV_Target
{
    // 알파값구분만 색상 변경
    //if (vColor.a <= 0.1f)
    //{
    //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
    //}
    
    // 회색 쉐이더
    //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    //vColor.rgb = float3(Aver, Aver, Aver);
    //vColor.a = 1.f;
    
    //// 마젠타 색상 제거
    //if (vColor.r >= 1.f && vColor.b >= 1.f)
    //{
    //    clip(-1);
    //}
    
    //    //uint width = 0;
    ////uint height = 0;
    ////g_tex_1.GetDimensions(width, height);
    
    //float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    //if (g_btex_0)
    //{
    //    vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
    //    //saturate 0 ~ 1 을 넘지 않게 보정
    //    float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
    //    if (fAlpha < 0.1f)
    //    {
    //        // 픽셀 쉐이더를 중간에 폐기처리
    //        discard; //clip(-1);            
    //    }
    //}
  
    return g_UseTexture ? g_tex_0.Sample(g_LinearSampler, _in.vUV) : _in.vColor;
}

#endif
