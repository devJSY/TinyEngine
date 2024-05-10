#ifndef _DECAL
#define _DECAL

#include "struct.hlsli"
#include "global.hlsli"

// ==========================
// Decal Shader
// Domain   : DOMAIN_DECAL
// MRT      : Decal
// RS_TYPE  : CULL_FRONT
// DS_TYPE  : NoTest NoWrite
// BS_TYPE  : DECAL 

// Parameter
// g_int_0  : AsEmissive
// g_mat_0  : WorldInv
// g_tex_0  : Output Texture
// g_tex_1  : PositionTargetTex
// ===========================

PS_IN VS_Decal(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;
    
    output.vPosProj = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vEmissive : SV_Target1;
};

PS_OUT PS_Decal(PS_IN _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // 호출된 픽셀의 위치를 UV 값으로 환산
    float2 vScreenUV = _in.vPosProj.xy / g_RenderResolution;
    
    // PositionTarget 에서 현재 호출된 픽셀쉐이더랑 동일한 지점에 접근해서 좌표값을 확인
    float4 vWorldPos = g_tex_1.Sample(g_LinearWrapSampler, vScreenUV);
    
    // x,y,z 전부 0이라면 discard
    if (!any(vWorldPos))
    {
        discard;
    }
    
    // Cube 볼륨메쉬의 로컬 공간으로 데려간다.
    float3 vLocal = mul(float4(vWorldPos.xyz, 1.f), g_mat_0).xyz;
    if (abs(vLocal.x) > 1.f || abs(vLocal.y) > 1.f || abs(vLocal.z) > 1.f)
    {
        discard;
    }
    
    // -1 ~ 1 → 0 ~ 1 범위 변경 
    vLocal = (vLocal + 1.f) * 0.5f;
    
    output.vColor = float4(0.f, 0.f, 0.f, 0.f);
    output.vEmissive = float4(0.f, 0.f, 0.f, 1.f);
    
    // 볼륨메쉬 내부 판정 성공 시
    if (g_btex_0)
    {
        output.vColor = g_tex_0.Sample(g_LinearWrapSampler, vLocal.xz);
        
        if (g_int_0)
        {
            output.vEmissive.rgb = output.vColor.rgb * output.vColor.a;
        }
    }
    
    return output;
}

#endif