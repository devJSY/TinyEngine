#include "struct.hlsli"
#include "global.hlsli"

#define Thickness g_int_0
#define OutLineColor g_vec4_0

float4 main(PS_IN input) : SV_TARGET
{
    float4 color = float4(0.f, 0.f, 0.f, 1.f);
    float dx = 1.f / g_RenderResolution.x;
    float dy = 1.f / g_RenderResolution.y;
    
    if (g_UseAnim2D)
    {
        float2 vUV = float2(0.0, 0.0);
        dx *= g_vSliceSize.x; 
        dy *= g_vSliceSize.y; 

        if (g_UseBackGround)
        {
            float2 vBackgroundLeftTop = g_vLeftTop + (g_vSliceSize / 2.f) - (g_vBackGround / 2.f);
            vBackgroundLeftTop -= g_vOffset;
            vUV = vBackgroundLeftTop + (g_vBackGround * input.vUV);
        }
        else
        {
            float2 LT = g_vLeftTop - g_vOffset;
            vUV = LT + (g_vSliceSize * input.vUV);
        }
        
        float4 AnimColor = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV);
                
        // 알파값이 0이면 외곽선 체크
        if (0.f >= AnimColor.a)
        {
            float totalAlpha = 0.f;
        
            // 8방향 탐색 * Thickness
            [unroll(50)] // 최대 50픽셀 제한
            for (int i = 1; i <= Thickness; ++i)
            {
                float4 pixelTop = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(0.f, -dy * i));
                float4 pixelBottom = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(0.f, dy * i));
                float4 pixelRight = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(dx * i, 0.f));
                float4 pixelLeft = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(-dx * i, 0.f));
        
                float4 pixelLT = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(-dx * i, -dy * i));
                float4 pixelLB = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(-dx * i, dy * i));
                float4 pixelRT = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(dx * i, -dy * i));
                float4 pixelRB = g_anim2d_tex.Sample(g_LinearWrapSampler, vUV + float2(dx * i, dy * i));
        
                totalAlpha += pixelTop.a + pixelBottom.a + pixelRight.a + pixelLeft.a + pixelLT.a + pixelLB.a + pixelRT.a + pixelRB.a;
            }
            
            // 8방향 탐색 후 비어있는 영역이라면 해당 픽셀은 폐기처리
            if (0.f >= totalAlpha)
                discard;
    
            color = OutLineColor;
        }
        else
        {
            // 샘플링해온 지점이 색상이 있었다면 해당 색상으로 렌더링 
            color = AnimColor;
        }
    }
    else if (g_btex_0)
    {
        float4 TexColor = g_tex_0.Sample(g_LinearWrapSampler, input.vUV);
        
        // 알파값이 0이면 외곽선 체크
        if (0.f >= TexColor.a)
        {
            float totalAlpha = 0.f;
    
            // 8방향 탐색 * Thickness
            [unroll(50)] // 최대 50픽셀 제한
            for (int i = 1; i <= Thickness; ++i)
            {
                float4 pixelTop = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(0.f, -dy * i));
                float4 pixelBottom = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(0.f, dy * i));
                float4 pixelRight = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(dx * i, 0.f));
                float4 pixelLeft = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(-dx * i, 0.f));
        
                float4 pixelLT = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(-dx * i, -dy * i));
                float4 pixelLB = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(-dx * i, dy * i));
                float4 pixelRT = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(dx * i, -dy * i));
                float4 pixelRB = g_tex_0.Sample(g_LinearWrapSampler, input.vUV + float2(dx * i, dy * i));
        
                totalAlpha += pixelTop.a + pixelBottom.a + pixelRight.a + pixelLeft.a + pixelLT.a + pixelLB.a + pixelRT.a + pixelRB.a;
            }
    
            // 8방향 탐색 후 비어있는 영역이라면 해당 픽셀은 폐기처리
            if (0.f >= totalAlpha)
                discard;
    
            color = OutLineColor;
        }
        else
        {
            // 샘플링해온 지점이 색상이 있었다면 해당 색상으로 렌더링 
            color = TexColor;
        }
    }
    else
    {
        discard;
    }
    
    color.a = 1.f;
    return
color;
}