#include "global.hlsli"
#include "struct.hlsli"
#include "func.hlsli"

#define BlurTex g_tex_0

#define MinDistance g_float_0
#define MaxDistance g_float_1
#define FocusUV g_vec2_0

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = g_postprocess_Tex.Sample(g_LinearClampSampler, input.vUV0).rgb;
    
    // Target이 화면 밖인경우
    if (FocusUV.x < 0.f || FocusUV.y < 0.f || FocusUV.x > 1.f || FocusUV.y > 1.f)
    {
        return float4(color, 1.f);
    }
    
    float3 outOfFocusColor = BlurTex.Sample(g_LinearClampSampler, input.vUV0).rgb;
    
    float PosZ = TexcoordToView(DepthOnlyTex, input.vUV0).z;
    float FocusPosZ = TexcoordToView(DepthOnlyTex, FocusUV).z;
    
    // 깊이 값에 따른 Blur 강도 조정
    float blur = smoothstep(MinDistance, MaxDistance, abs(PosZ - FocusPosZ));
    color = lerp(color, outOfFocusColor, blur);

    return float4(color, 1.f);
}