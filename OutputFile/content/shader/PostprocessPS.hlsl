#include "global.hlsli"
#include "struct.hlsli"

float4 main(PS_IN input) : SV_TARGET
{
    float4 texColor = g_Missing_tex.Sample(g_LinearSampler, input.vUV);
    
    if (g_btex_1)
    {
        int width = 0;
        int height = 0;
        int numMips = 0;
        
        // 2중 예외처리
        // 바인딩된 텍스춰가 실제로 존재할 경우에만 샘플링 
        g_tex_1.GetDimensions(0, width, height, numMips);
        if (!(0 == width || 0 == height))
            texColor = g_tex_1.Sample(g_PointSampler, input.vUV);
    }
    
    return texColor;
}