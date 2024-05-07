#include "struct.hlsli"
#include "global.hlsli"

// reference by https://learnopengl.com/Advanced-Lighting/SSAO
// reference by https://nellfamily.tistory.com/48
// reference by https://betterprogramming.pub/depth-only-ssao-for-forward-renderers-1a3dcfa1873a


#define DepthOnlyTex g_tex_0
#define PositionTex g_tex_1
#define NormalTex g_tex_2

#define SampleRadius g_float_0
#define PowPower g_float_1

#define MAX_KERNEL_SIZE 16
#define INV_MAX_KERNEL_SIZE_F 1.f / float(MAX_KERNEL_SIZE);

static const float bias = 0.025f;

static const float3 g_kernel[MAX_KERNEL_SIZE] =
{
    float3(0.2024537f, 0.841204f, -0.9060141f),
    float3(-0.2200423f, 0.6282339f, -0.8275437f),
    float3(0.3677659f, 0.1086345f, -0.4466777f),
    float3(0.8775856f, 0.4617546f, -0.6427765f),
    float3(0.7867433f, -0.141479f, -0.1567597f),
    float3(0.4839356f, -0.8253108f, -0.1563844f),
    float3(0.4401554f, -0.4228428f, -0.3300118f),
    float3(0.001993f, -0.8048455f, 0.0726584f),
    float3(-0.7578573f, -0.5583301f, 0.2347527f),
    float3(-0.450417f, -0.252365f, 0.0694318f),
    float3(-0.0483353f, -0.2527294f, 0.592475f),
    float3(0.4192392f, 0.2084218f, -0.3672943f),
    float3(-0.8433938f, 0.1451271f, 0.2202872f),
    float3(-0.4037157f, -0.8263387f, 0.4698132f),
    float3(-0.6657394f, 0.6298575f, 0.6342437f),
    float3(-0.0001783f, 0.2834622f, 0.8343929f),
};

float4 TexcoordToView(float2 texcoord)
{
    float4 posProj;

    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
    posProj.xy = texcoord * 2.f - 1.f;
    posProj.y *= -1; // 주의: y 방향을 뒤집어줘야 합니다.
    posProj.z = DepthOnlyTex.Sample(g_LinearClampSampler, texcoord).r;
    posProj.w = 1.f;

    // ProjectSpace -> ViewSpace
    float4 posView = mul(posProj, g_matProjInv);
    posView.xyz /= posView.w;
    
    return posView;
}

float4 main(PS_IN input) : SV_TARGET
{
    // 뷰 포지션
    float3 viewPos = PositionTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    viewPos = mul(float4(viewPos, 1.f), g_matView).rgb;
    
    // 뷰 노말
    float3 viewNormal = NormalTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    viewNormal = normalize(mul(float4(viewNormal, 0.f), g_matView).rgb);

    // 랜덤 벡터 생성
    float2 NoiseScale = float2(g_NoiseTexResolution.x / 4.f, g_NoiseTexResolution.y / 4.f);
    float3 randomVec = g_NoiseTex.Sample(g_LinearClampSampler, input.vUV * NoiseScale).rgb;
    randomVec = normalize(randomVec * 2.f - 1.f);

    // TBN 행렬 생성
    float3 tangent = normalize(randomVec - viewNormal * dot(randomVec, viewNormal)); // Gram-Schmidt orthonormalization 
    float3 bitangent = normalize(cross(viewNormal, tangent));
    float3x3 TBN = float3x3(tangent, bitangent, viewNormal);
    
    // occlusion 누적 량
    float occlusion_factor = 0.f;
    
    [unroll] 
    for (int i = 0; i < MAX_KERNEL_SIZE; i++)
    {
        float3 samplePos = mul(g_kernel[i], TBN);

        // 반구 영역의 랜덤 위치 추출
        samplePos = viewPos + samplePos * SampleRadius;

        // View Coordinate → NDC Coordinate
        float4 offset = float4(samplePos, 1.f);
        offset = mul(offset, g_matProj);
        offset.xy /= offset.w;
        
        // NDC Coordinate → Texture Coordinate
        offset.y *= -1.f;
        offset.xy = offset.xy * float2(0.5f, 0.5f) + float2(0.5f, 0.5f);

        // 랜덤 위치의 깊이값 추출
        float geometryDepth = TexcoordToView(offset.xy).z;
  
        float rangeCheck = smoothstep(0.f, 1.f, SampleRadius / abs(viewPos.z - geometryDepth));
            
        // occlusion 결과값 누적
        occlusion_factor += float(geometryDepth >= samplePos.z + bias) * rangeCheck;
    }

    // 누적값의 평균 계산
    float average_occlusion_factor = occlusion_factor * INV_MAX_KERNEL_SIZE_F;
            
    float visibility_factor = 1.f - average_occlusion_factor;

    // occlusion 강도 조정
    visibility_factor = pow(visibility_factor, PowPower);
    
    return float4(visibility_factor, visibility_factor, visibility_factor, 1.f);
}
