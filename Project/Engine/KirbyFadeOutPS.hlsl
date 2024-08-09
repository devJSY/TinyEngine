#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define MaskTexture g_tex_0

#define Weight g_float_0
#define RotateAngle g_float_1

#define TargetNDCPos g_vec2_0
#define BackGroundColor g_vec4_0

float4 main(PS_IN _in) : SV_Target
{
    float3 color = g_postprocess_Tex.Sample(g_LinearClampSampler, _in.vUV0).rgb;
    
    if (!g_btex_0)
    {
        return float4(color, 1.f);
    }
    
    // Target이 화면 밖인경우
    if (TargetNDCPos.x < -1.f || TargetNDCPos.y < -1.f || TargetNDCPos.x > 1.f || TargetNDCPos.y > 1.f)
    {
        return float4(BackGroundColor.rgb, 1.f);
    }
    
    // UV → NDC 
    _in.vUV0 = float2(_in.vUV0.x, 1.f - _in.vUV0.y);
    _in.vUV0 *= 2.f;
    _in.vUV0 -= 1.f;
    
    // Translation
    float2 LerpValue = lerp(float2(0.f, 0.f), TargetNDCPos, Weight);
    _in.vUV0 -= LerpValue;
    
    // Scale
    _in.vUV0 *= 1.f / (1.f - Weight);
    
     // NDC → UV 
    _in.vUV0 = float2(_in.vUV0.x, -_in.vUV0.y);
    _in.vUV0 += 1.f;
    _in.vUV0 *= 0.5f;
    
    // AspectRatio 보정
    _in.vUV0 = AspectRatioCorrection(_in.vUV0);
    
    // Rotation
    _in.vUV0 = RotateUV(_in.vUV0, RotateAngle);
    
    float MaskArea = g_tex_0.Sample(g_LinearClampSampler, _in.vUV0).a;
    if (MaskArea < 0.1f)
    {
        return float4(g_vec4_0.rgb, 1.f);
    }
    
    return float4(color, 1.f);
}