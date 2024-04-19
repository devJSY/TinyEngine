#include "struct.hlsli"
#include "global.hlsli"

#define DepthOnlyTex g_tex_0
#define SampleRadius g_float_0

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
    posProj.xy = texcoord * 2.0 - 1.0;
    posProj.y *= -1; // 주의: y 방향을 뒤집어줘야 합니다.
    posProj.z = DepthOnlyTex.Sample(g_LinearClampSampler, texcoord).r;
    posProj.w = 1.0;

    // ProjectSpace -> ViewSpace
    float4 posView = mul(posProj, g_matProjInv);
    posView.xyz /= posView.w;
    
    return posView;
}

float4 main(PS_IN input) : SV_TARGET
{
    float3 viewPos = TexcoordToView(input.vUV).rgb;

    // the dFdy and dFdX are glsl functions used to calculate two vectors in view space 
    // that lie on the plane of the surface being drawn. We pass the view space position to these functions.
    // The cross product of these two vectors give us the normal in view space.
    float3 viewNormal = cross(ddy(viewPos.xyz), ddx(viewPos.xyz));

    // The normal is initilly away from the screen based on the order in which we calculate the cross products. 
    // Here, we need to invert it to point towards the screen by multiplying by -1. 
    // Then we normalize this vector to get a unit normal vector.
    viewNormal = normalize(viewNormal * -1.f);
    // we calculate a random offset using the noise texture sample. 
    //This will be applied as rotation to all samples for our current fragments.
    float3 randomVec = g_NoiseTex.Sample(g_PointSampler, input.vUV).xyz;
    // here we apply the Gramm-Schmidt process to calculate the TBN matrix 
    // with a random offset applied. 
    float3 tangent = normalize(randomVec - viewNormal * dot(randomVec, viewNormal));
    float3 bitangent = cross(viewNormal, tangent);
    float3x3 TBN = float3x3(tangent, bitangent, viewNormal);
    float occlusion_factor = 0.0;
    for (int i = 0; i < MAX_KERNEL_SIZE; i++)
    {
        float3 samplePos = mul(g_kernel[i], TBN);

        // here we calculate the sampling point position in view space.
        samplePos = viewPos + samplePos * SampleRadius;

        // now using the sampling point offset
        float4 offset = float4(samplePos, 1.0);
        offset = mul(offset, g_matProj);
        offset.xy /= offset.w;
        offset.xy = offset.xy * float2(0.5f, 0.5f) + float2(0.5f, 0.5f);

        // this is the geometry's depth i.e. the view_space_geometry_depth
        // this value is negative in my coordinate system
        float geometryDepth = TexcoordToView(offset.xy).z;
            
        float rangeCheck = smoothstep(0.0, 1.0, SampleRadius / abs(viewPos.z - geometryDepth));
            
        // samplePos.z is the sample's depth i.e. the view_space_sampling_position depth
        // this value is negative in my coordinate system
        // for occlusion to be true the geometry's depth should be greater or equal (equal or less negative and consequently closer to the camera) than the sample's depth
        occlusion_factor += float(geometryDepth >= samplePos.z + 0.0001) * rangeCheck;
    }

    // we will devide the accmulated occlusion by the number of samples to get the average occlusion value. 
    float average_occlusion_factor = occlusion_factor * INV_MAX_KERNEL_SIZE_F;
        
    float visibility_factor = 1.0 - average_occlusion_factor;

    // We can raise the visibility factor to a power to make the transition
    // more sharp. Experiment with the value of this power to see what works best for you.
    // Even after raising visibility to a power > 1, the range still remains between [0.0, 1.0].
    visibility_factor = pow(visibility_factor, 2.0);

    return float4(visibility_factor, visibility_factor, visibility_factor, 1.f);
}