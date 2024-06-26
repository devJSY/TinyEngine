#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

#define AmbientTex g_tex_0
#define MRATex g_tex_1
#define NormalTex g_tex_2
#define HeightTexture g_tex_3
#define EmissiveTex g_tex_4

#define MtrlAlbedo g_vAlbedo
#define MtrlMetallic g_vMetallic
#define MtrlRoughness g_vRoughness
#define MtrlEmission g_vEmission

#define TileTexArr g_texArr_0  // Tile 배열 택스쳐
#define TileTexArrSize g_float_0   // 배열 개수
#define WeightMapResolution g_vec2_0    // 가중치 버퍼 해상도
StructuredBuffer<float4> WEIGHT_MAP : register(t17); // 가중치 버퍼

#define Tess_Dist_Scale g_float_1   // 텍셀레이션 거리 스케일

// =======================================
// LandScape Shader
// MRT      : Deferred
// RS_TYPE  : CULL_BACK
// DS_TYPE  : LESS
// BS_TYPE  : DEFAULT

// Parameter
// g_int_0  : Face X
// g_int_1  : Face Z
// g_tex_3  : HeightMap Texture
// =======================================

struct VS_Input
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
};

struct VS_Output
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

VS_Output VS_LandScape(VS_Input _in)
{
    VS_Output output = (VS_Output) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vNormal = _in.vNormal;
    output.vTangent = _in.vTangent;
    output.vBinormal = cross(output.vNormal, output.vTangent);
    
    return output;
}

// Hull Shader
struct PatchLevel
{
    float arrEdge[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

PatchLevel PatchConstFunc(InputPatch<VS_Output, 3> _in, uint patchID : SV_PrimitiveID)
{
    PatchLevel output = (PatchLevel) 0.f;
        
    float3 vUpDown = (_in[1].vWorldPos + _in[2].vWorldPos) / 2.f;
    float3 vLeftRight = (_in[0].vWorldPos + _in[2].vWorldPos) / 2.f;
    float3 vSlide = (_in[0].vWorldPos + _in[1].vWorldPos) / 2.f;
    float3 vMid = (_in[0].vWorldPos + _in[1].vWorldPos + _in[2].vWorldPos) / 3.f;

    float3 vCamWorldPos = g_eyeWorld;
    vCamWorldPos.y = 0.f;
    
    output.arrEdge[0] = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vUpDown), 1, 4, 10.f * Tess_Dist_Scale, 40.f * Tess_Dist_Scale));
    output.arrEdge[1] = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vLeftRight), 1, 4, 10.f * Tess_Dist_Scale, 40.f * Tess_Dist_Scale));
    output.arrEdge[2] = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vSlide), 1, 4, 10.f * Tess_Dist_Scale, 40.f * Tess_Dist_Scale));
    output.Inside = pow(2, (int) GetTessFactor(distance(vCamWorldPos, vMid), 1, 4, 10.f * Tess_Dist_Scale, 40.f * Tess_Dist_Scale));
    
    return output;
}

struct HS_Output
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vNormal : NORMAL;
};

[patchconstantfunc("PatchConstFunc")]
[outputtopology("triangle_cw")]
[domain("tri")]
[maxtessfactor(64)]
[partitioning("integer")] //[partitioning("fractional_odd")]
[outputcontrolpoints(3)]
HS_Output HS_LandScape(InputPatch<VS_Output, 3> _in, uint _idx : SV_OutputControlPointID)
{
    HS_Output output = (HS_Output) 0.f;
    
    output.vPos = _in[_idx].vPos;
    output.vUV = _in[_idx].vUV;
    
    output.vTangent = _in[_idx].vTangent;
    output.vBinormal = _in[_idx].vBinormal;
    output.vNormal = _in[_idx].vNormal;
    
    return output;
}

struct DS_Output
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float2 vFullUV : TEXCOORD1;
        
    float3 vWorldPos : POSITION;
    float3 vWorldTangent : TANGENT;
    float3 vWorldBinormal : BINORMAL;
    float3 vWorldNormal : NORMAL;
};

[domain("tri")]
DS_Output DS_LandScape(PatchLevel _pathlevel // 각 제어점 별 분할 레벨
             , const OutputPatch<HS_Output, 3> _Origin // 패치 원본 정점
             , float3 _Weight : SV_DomainLocation)   // 각 원본 정점에 대한 가중치)
{
    DS_Output output = (DS_Output) 0.f;
        
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    float3 vTangent = (float3) 0.f;
    float3 vBinormal = (float3) 0.f;
    float3 vNormal = (float3) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _Origin[i].vPos * _Weight[i];
        vUV += _Origin[i].vUV * _Weight[i];
        
        vTangent += _Origin[i].vTangent * _Weight[i];
        vBinormal += _Origin[i].vBinormal * _Weight[i];
        vNormal += _Origin[i].vNormal * _Weight[i];
    }
    
    // 지형 전체기준 UV
    output.vFullUV = vUV / float2(g_int_0, g_int_1);
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    output.vWorldPos = mul(float4(vLocalPos, 1.f), g_matWorld).xyz;
    output.vWorldTangent = mul(float4(vTangent, 1.f), g_matWorld).xyz;
    output.vWorldBinormal = mul(float4(vBinormal, 1.f), g_matWorld).xyz;
    output.vWorldNormal = mul(float4(vNormal, 1.f), g_matWorld).xyz;
    
    // 높이맵 텍스춰가 존재하는 경우
    if (g_btex_3)
    {
        float2 FullUV = output.vFullUV;
        vLocalPos.y = HeightTexture.SampleLevel(g_LinearClampSampler, FullUV, 0).x;
                       
        // 주변 정점(위, 아래, 좌, 우) 로 접근할때의 로컬스페이스상에서의 간격
        float LocalStep = 1.f / _pathlevel.Inside;
        
        // 주변 정점(위, 아래, 좌, 우) 의 높이를 높이맵에서 가져올때 중심UV 에서 주변UV 로 접근할때의 UV 변화량
        float2 vUVStep = LocalStep / float2(g_int_0, g_int_1);
        
        // 위
        float3 vUp = float3(vLocalPos.x
                            , HeightTexture.SampleLevel(g_LinearClampSampler, float2(FullUV.x, FullUV.y - vUVStep.y), 0).x
                            , vLocalPos.z + LocalStep);
        
        // 아래
        float3 vDown = float3(vLocalPos.x
                             , HeightTexture.SampleLevel(g_LinearClampSampler, float2(FullUV.x, FullUV.y + vUVStep.y), 0).x
                             , vLocalPos.z - LocalStep);
        
        // 좌
        float3 vLeft = float3(vLocalPos.x - LocalStep
                             , HeightTexture.SampleLevel(g_LinearClampSampler, float2(FullUV.x - vUVStep.x, FullUV.y), 0).x
                             , vLocalPos.z);
        
        // 우
        float3 vRight = float3(vLocalPos.x + LocalStep
                            , HeightTexture.SampleLevel(g_LinearClampSampler, float2(FullUV.x + vUVStep.x, FullUV.y), 0).x
                            , vLocalPos.z);
        
        output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
        output.vWorldPos = mul(float4(vLocalPos, 1.f), g_matWorld).xyz;
        output.vWorldTangent = normalize(mul(float4(vRight, 1.f), g_matWorld).xyz - mul(float4(vLeft, 1.f), g_matWorld).xyz);
        output.vWorldBinormal = normalize(mul(float4(vDown, 1.f), g_matWorld).xyz - mul(float4(vUp, 1.f), g_matWorld).xyz);
        output.vWorldNormal = normalize(cross(output.vWorldTangent, output.vWorldBinormal));
    }
        
    return output;
}

struct PS_Output
{
    float4 vColor : SV_Target0;
    float4 vPosition : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vTangent : SV_Target3;
    float4 vBitangent : SV_Target4;
    float4 vEmissive : SV_Target5;
    float4 vMRA : SV_Target6;
};

PS_Output PS_LandScape(DS_Output _in)
{
    PS_Output output = (PS_Output) 0.f;
    
    float3 albedo = g_btex_0 ? AmbientTex.Sample(g_LinearWrapSampler, _in.vUV).rgb 
                                 : MtrlAlbedo.rgb;
    float3 normal = _in.vWorldNormal;
    float metallic = g_btex_1 ? MRATex.Sample(g_LinearWrapSampler, _in.vUV).r
                                    : MtrlMetallic;
    float roughness = g_btex_1 ? MRATex.Sample(g_LinearWrapSampler, _in.vUV).g 
                                      : MtrlRoughness;
    float ao = g_btex_1 ? MRATex.Sample(g_LinearWrapSampler, _in.vUV).b : 1.f;
    if (ao >= 1.f)
    {
        ao = SSAOTex.Sample(g_LinearWrapSampler, _in.vUV).r;
    }
    float3 emission = g_btex_4 ? EmissiveTex.Sample(g_LinearWrapSampler, _in.vUV).rgb
                                     : MtrlEmission.rgb;

    if (g_btexarr_0)
    {
        float2 derivX = ddx(_in.vUV); // 인접픽셀과 x축 편미분값을 구한다
        float2 derivY = ddy(_in.vUV); // 인접픽셀과 y축 편미분값을 구한다

        // 타일 색상
        int2 iWeightIdx = (int2) (_in.vFullUV * WeightMapResolution);
        float4 vWeight = WEIGHT_MAP[iWeightIdx.y * (int) WeightMapResolution.x + iWeightIdx.x];
        float4 vColor = (float4) 0.f;

        int iMaxWeightIdx = -1;
        float fMaxWeight = 0.f;

        for (int i = 0; i < TileTexArrSize; ++i)
        {
            // 배열 텍스쳐 샘플링할때 UV 3번째값이 배열 인덱스
            vColor += TileTexArr.SampleGrad(g_LinearWrapSampler, float3(_in.vUV, i), derivX, derivY) * vWeight[i];
            //vColor += TileTexArr.SampleLevel(g_LinearWrapSampler, float3(_in.vUV, i), 0) * vWeight[i];

            if (fMaxWeight < vWeight[i])
            {
                fMaxWeight = vWeight[i];
                iMaxWeightIdx = i;
            }
        }
        
        albedo = vColor.rgb;

        // 타일 노말
        if (-1 != iMaxWeightIdx)
        {
            float3 vTangentSpaceNormal = TileTexArr.SampleGrad(g_LinearWrapSampler, float3(_in.vUV, iMaxWeightIdx + TileTexArrSize), derivX, derivY).xyz;
            //float3 vTangentSpaceNormal = TileTexArr.SampleLevel(g_LinearWrapSampler, float3(_in.vUV, iMaxWeightIdx + TileCount), 0).xyz;
            vTangentSpaceNormal = vTangentSpaceNormal * 2.f - 1.f;

            float3x3 matTBN = { _in.vWorldTangent, _in.vWorldBinormal, _in.vWorldNormal };
            normal = normalize(mul(vTangentSpaceNormal, matTBN));
        }
    }
    
    output.vColor = float4(albedo, 1.f);
    output.vPosition = float4(_in.vWorldPos, 1.f);
    output.vTangent = float4(_in.vWorldTangent, 1.f);
    output.vBitangent = float4(_in.vWorldBinormal, 1.f);
    output.vNormal = float4(normal, 1.f);
    output.vEmissive = float4(emission, 1.f);
    output.vMRA = float4(metallic, roughness, ao, 1.f);
    
    return output;
}

#endif