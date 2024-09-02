#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define EmissiveNosieTex g_tex_0
#define AlphaNosieTex g_tex_1

#define RenderMaskFlag g_int_0 // (0) NONE, (1) LocalZ, (2) WorldY
#define bRenderMaskUseFrom g_vec4_0.x // (x) Use From Pos : (0) NONE
#define bRenderMaskUseTo g_vec4_0.y // (y) Use To Pos : (0) NONE
#define RenderMaskFrom g_vec4_0.z // (z) From Pos
#define RenderMaskTo g_vec4_0.w // (w) To Pos

#define MtrlAlbedo g_vAlbedo
#define MtrlEmission g_vEmission

float4 main(PS_IN _in) : SV_Target0
{
    float3 OutColor = MtrlAlbedo.rgb;
    float Alpha = 1.f;
    
    // -------------------
    // Masking
    // -------------------
    // Masking : Local X
    if (RenderMaskFlag == 1)
    {
        float4 LocalPos = mul(float4(_in.vPosWorld, 1.f), g_matWorldInv);
        
        // Use From
        if (bRenderMaskUseFrom > 0.f)
        {
            if (LocalPos.z < RenderMaskFrom)
            {
                Alpha = 0.f;
            }
        }
        // Use To
        if (bRenderMaskUseTo > 0.f)
        {
            if (LocalPos.z > RenderMaskTo)
            {
                Alpha = 0.f;            
            }
        }
    }
    
    // Masking : World Y
    else if (RenderMaskFlag == 2)
    {
        // Use From
        if (bRenderMaskUseFrom > 0.f)
        {
            if (_in.vPosWorld.y < RenderMaskFrom)
            {
                Alpha = 0.f;
            }
        }
        // Use To
        if (bRenderMaskUseTo > 0.f)
        {
            if (_in.vPosWorld.y > RenderMaskTo)
            {
                Alpha = 0.f;
            }
        }
    }
    
    if (Alpha <= 0.f)
        clip(-1);
    
    // -------------------
    // Render
    // -------------------
    // emissive noise
    if (g_btex_0)
    {
        float2 MoveUV = _in.vUV0 + g_EngineTime * 5.f;
        float Emissive = EmissiveNosieTex.Sample(g_LinearWrapSampler, MoveUV).r;
        OutColor = OutColor * (1.f - Emissive) + MtrlEmission.rgb * Emissive;
    }
    
    return float4(OutColor, Alpha);
}
