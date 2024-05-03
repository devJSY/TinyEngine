#ifndef _LIGHT_DEFERRED
#define _LIGHT_DEFERRED

#include "global.hlsli"
#include "Light.hlsli"

#define MtrlAlbedo g_vAlbedo
#define MtrlDiffuse g_vDiffuse
#define MtrlSpecular g_vSpecular

// ========================
// Directional Light Shader
// MRT      : LIGHT
// Mesh     : RectMesh
// DS_TYPE  : NO_TEST_NO_WIRTE
// BS_TYPE  : ONE_ONE , 여러개의 빛이 누적될 수 있게

// Parameter
// g_int_0 : Light Idex
// g_tex_0 : PositionTargetTex
// g_tex_1 : NormalTargetTex
// ========================

PS_IN VS_DirLight(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;
    
    output.vPosProj = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

struct PS_OUT
{
    float4 vDiffuse : SV_Target0;
    float4 vSpecular : SV_Target1;
};

PS_OUT PS_DirLight(PS_IN _in)
{
    PS_OUT output = (PS_OUT) 0.f;
        
    // PositionTarget 에서 현재 호출된 픽셀쉐이더랑 동일한 지점에 접근해서 좌표값을 확인
    float4 vWorldPos = g_tex_0.Sample(g_LinearWrapSampler, _in.vUV);
    
    // Deferred 단계에서 그려진게 없다면 빛을 줄 수 없다.
    if (!any(vWorldPos))
        discard;
    
    // 해당 지점의 Normal 값을 가져온다.
    float3 vWorldNormal = normalize(g_tex_1.Sample(g_LinearWrapSampler, _in.vUV).xyz);
       
    // 해당 지점이 받을 빛의 세기를 구한다.
    tLightColor LightColor = (tLightColor) 0.f;
    CalculateLight3D(g_int_0, vWorldPos.xyz, vWorldNormal, LightColor);
        
    output.vDiffuse = LightColor.vDiffuse;
    output.vSpecular = LightColor.vSpecular;
    
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}

#endif