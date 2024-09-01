#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define BlackNosieTex g_tex_0
#define AlphaNosieTex g_tex_1
#define EmissiveNosieTex g_tex_2

#define RenderMaskFlag g_int_0 // (0) NONE, (1) LocalZ, (2) WorldY
#define bRenderMaskUseFrom g_vec4_0.x // (x) Use From Pos : (0) NONE
#define bRenderMaskUseTo g_vec4_0.y // (y) Use To Pos : (0) NONE
#define RenderMaskFrom g_vec4_0.z // (z) From Pos
#define RenderMaskTo g_vec4_0.w // (w) To Pos

#define MtrlAlbedo g_vAlbedo
#define MtrlEmission g_vEmission

float4 main(PS_IN _in) : SV_Target0
{
    float3 OutColor = MtrlEmission.rgb;
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
    // ¿Ü°ûÃ³¸®
    float3 pixelToEye = normalize(g_eyeWorld - _in.vPosWorld);
    float3 normalWorld = _in.vNormalWorld;
    Alpha = 1.f - pow(saturate(abs(dot(-pixelToEye, normalWorld))), 0.1f);
    
    // emissive noise
    if (g_btex_0)
    {
        float2 MoveUV = _in.vUV0 * 2.f + g_EngineTime * 9.f;
        float Emissive = EmissiveNosieTex.Sample(g_LinearWrapSampler, MoveUV).r;
        
        if (Emissive > 0.1f)
        {
            Alpha = Emissive;
        }
    }
    
    // black noise
    if (g_btex_0)
    {
        float2 MoveUV = _in.vUV0 * 1.f + g_EngineTime * 9.f;
        float Black = BlackNosieTex.Sample(g_LinearWrapSampler, MoveUV).r;
        Black = pow(abs(Black), 0.1f);
        
        if (Black > 0.9f)
        {
            OutColor = (float3) 0.f;
            Alpha = 1.f;
        }
    }
    
    if (Alpha <= 0.f)
        clip(-1);
    
    return float4(OutColor, Alpha);
}
