#ifndef _STD2D
#define _STD2D

#include "global.hlsli"
#include "struct.hlsli"

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosProj = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_LinearSampler, _in.vUV);
    
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
    
    //return _in.vColor;
    
    return vColor;
}

#endif
