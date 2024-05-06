#include "struct.hlsli"
#include "global.hlsli"

#define DepthOnlyTex g_tex_0
#define PositionTex g_tex_1
#define NormalTex g_tex_2
#define SampleRadius g_float_0
#define PowPower g_float_1

#define MAX_KERNEL_SIZE 16
#define INV_MAX_KERNEL_SIZE_F 1.f / float(MAX_KERNEL_SIZE);

float3 g_kernel[MAX_KERNEL_SIZE] =
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
    float3 viewPos = PositionTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    viewPos = mul(float4(viewPos, 1.f), g_matView).rgb;
    
    float3 viewNormal = NormalTex.Sample(g_LinearWrapSampler, input.vUV).rgb;
    viewNormal = normalize(mul(float4(viewNormal, 0.f), g_matView).rgb);

    // we calculate a random offset using the noise texture sample. 
    //This will be applied as rotation to all samples for our current fragments.
    float2 noiseScale = float2(g_NoiseTexResolution.x / 4.f, g_NoiseTexResolution.y / 4.f);
    float3 randomVec = normalize(g_NoiseTex.Sample(g_LinearWrapSampler, input.vUV * noiseScale).xyz);
    
    // here we apply the Gramm-Schmidt process to calculate the TBN matrix 
    // with a random offset applied. 
    float3 tangent = normalize(randomVec - viewNormal * dot(randomVec, viewNormal));
    float3 bitangent = normalize(cross(viewNormal, tangent));
    float3x3 TBN = float3x3(tangent, bitangent, viewNormal);
    
    float occlusion_factor = 0.f;
    for (int i = 0; i < MAX_KERNEL_SIZE; i++)
    {
        float3 samplePos = mul(g_kernel[i], TBN);

        // here we calculate the sampling point position in view space.
        samplePos = viewPos + samplePos * SampleRadius;

        // now using the sampling point offset
        float4 offset = float4(samplePos, 1.f);
        offset = mul(offset, g_matProj);
        offset.xy /= offset.w;
        offset.xy = offset.xy * float2(0.5f, 0.5f) + float2(0.5f, 0.5f); // transform to range 0.0 ~ 1.0
        offset.y *= -1.f;

        // this is the geometry's depth i.e. the view_space_geometry_depth
        // this value is negative in my coordinate system
        float geometryDepth = TexcoordToView(offset.xy).z;
            
        float rangeCheck = smoothstep(0.f, 1.f, SampleRadius / abs(viewPos.z - geometryDepth));
            
        // samplePos.z is the sample's depth i.e. the view_space_sampling_position depth
        // this value is negative in my coordinate system
        // for occlusion to be true the geometry's depth should be greater or equal (equal or less negative and consequently closer to the camera) than the sample's depth
        occlusion_factor += float(geometryDepth >= samplePos.z + 0.0001f) * rangeCheck;
    }

    // we will devide the accmulated occlusion by the number of samples to get the average occlusion value. 
    float average_occlusion_factor = occlusion_factor * INV_MAX_KERNEL_SIZE_F;
        
    float visibility_factor = 1.f - average_occlusion_factor;

    // We can raise the visibility factor to a power to make the transition
    // more sharp. Experiment with the value of this power to see what works best for you.
    // Even after raising visibility to a power > 1, the range still remains between [0.0, 1.0].
    visibility_factor = pow(visibility_factor, PowPower);

    return float4(visibility_factor, visibility_factor, visibility_factor, 1.f);
}
