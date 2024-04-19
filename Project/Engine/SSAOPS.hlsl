#include "struct.hlsli"
#include "global.hlsli"

#define DepthOnlyTex g_tex_0
#define SampleRadius g_float_0

//#define MAX_KERNEL_SIZE 16
//#define INV_MAX_KERNEL_SIZE_F 1.f / float(MAX_KERNEL_SIZE);

//float3 g_kernel[MAX_KERNEL_SIZE] =
//{
//    float3(0.2024537f, 0.841204f, -0.9060141f),
//    float3(-0.2200423f, 0.6282339f, -0.8275437f),
//    float3(0.3677659f, 0.1086345f, -0.4466777f),
//    float3(0.8775856f, 0.4617546f, -0.6427765f),
//    float3(0.7867433f, -0.141479f, -0.1567597f),
//    float3(0.4839356f, -0.8253108f, -0.1563844f),
//    float3(0.4401554f, -0.4228428f, -0.3300118f),
//    float3(0.001993f, -0.8048455f, 0.0726584f),
//    float3(-0.7578573f, -0.5583301f, 0.2347527f),
//    float3(-0.450417f, -0.252365f, 0.0694318f),
//    float3(-0.0483353f, -0.2527294f, 0.592475f),
//    float3(0.4192392f, 0.2084218f, -0.3672943f),
//    float3(-0.8433938f, 0.1451271f, 0.2202872f),
//    float3(-0.4037157f, -0.8263387f, 0.4698132f),
//    float3(-0.6657394f, 0.6298575f, 0.6342437f),
//    float3(-0.0001783f, 0.2834622f, 0.8343929f),
//};

//float4 TexcoordToView(float2 texcoord)
//{
//    float4 posProj;

//    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
//    posProj.xy = texcoord * 2.f - 1.f;
//    posProj.y *= -1; // 주의: y 방향을 뒤집어줘야 합니다.
//    posProj.z = DepthOnlyTex.Sample(g_LinearClampSampler, texcoord).r;
//    posProj.w = 1.f;

//    // ProjectSpace -> ViewSpace
//    float4 posView = mul(posProj, g_matProjInv);
//    posView.xyz /= posView.w;
    
//    return posView;
//}

//float4 main(PS_IN input) : SV_TARGET
//{
//    float3 viewPos = TexcoordToView(input.vUV).rgb;

//    // the dFdy and dFdX are glsl functions used to calculate two vectors in view space 
//    // that lie on the plane of the surface being drawn. We pass the view space position to these functions.
//    // The cross product of these two vectors give us the normal in view space.
//    float3 viewNormal = cross(ddy(viewPos.xyz), ddx(viewPos.xyz));

//    // The normal is initilly away from the screen based on the order in which we calculate the cross products. 
//    // Here, we need to invert it to point towards the screen by multiplying by -1. 
//    // Then we normalize this vector to get a unit normal vector.
//    viewNormal = normalize(viewNormal * -1.f);
//    // we calculate a random offset using the noise texture sample. 
//    //This will be applied as rotation to all samples for our current fragments.
//    float2 noiseScale = float2(g_RenderResolution.x / 4.f, g_RenderResolution.y / 4.f);
//    float3 randomVec = g_NoiseTex.Sample(g_PointSampler, input.vUV * noiseScale).xyz;
//    // here we apply the Gramm-Schmidt process to calculate the TBN matrix 
//    // with a random offset applied. 
//    float3 tangent = normalize(randomVec - viewNormal * dot(randomVec, viewNormal));
//    float3 bitangent = cross(viewNormal, tangent);
//    float3x3 TBN = float3x3(tangent, bitangent, viewNormal);
//    float occlusion_factor = 0.f;
//    for (int i = 0; i < MAX_KERNEL_SIZE; i++)
//    {
//        float3 samplePos = mul(g_kernel[i], TBN);

//        // here we calculate the sampling point position in view space.
//        samplePos = viewPos + samplePos * SampleRadius;

//        // now using the sampling point offset
//        float4 offset = float4(samplePos, 1.f);
//        offset = mul(offset, g_matProj);
//        offset.xy /= offset.w;
//        offset.xy = offset.xy * 0.5f + 0.5f;
//        offset.y *= -1.f;

//        // this is the geometry's depth i.e. the view_space_geometry_depth
//        // this value is negative in my coordinate system
//        float geometryDepth = TexcoordToView(offset.xy).z;
            
//        float rangeCheck = smoothstep(0.f, 1.f, SampleRadius / abs(viewPos.z - geometryDepth));
            
//        // samplePos.z is the sample's depth i.e. the view_space_sampling_position depth
//        // this value is negative in my coordinate system
//        // for occlusion to be true the geometry's depth should be greater or equal (equal or less negative and consequently closer to the camera) than the sample's depth
//        occlusion_factor += float(geometryDepth >= samplePos.z + 0.0001f) * rangeCheck;
//    }

//    // we will devide the accmulated occlusion by the number of samples to get the average occlusion value. 
//    float average_occlusion_factor = occlusion_factor * INV_MAX_KERNEL_SIZE_F;
        
//    float visibility_factor = 1.f - average_occlusion_factor;

//    // We can raise the visibility factor to a power to make the transition
//    // more sharp. Experiment with the value of this power to see what works best for you.
//    // Even after raising visibility to a power > 1, the range still remains between [0.0, 1.0].
//    visibility_factor = pow(visibility_factor, 2.f);

//    return float4(visibility_factor, visibility_factor, visibility_factor, 1.f);
//}

float3 normal_from_depth(float depth, float2 texcoords)
{
  
    const float2 offset1 = float2(0.0, 0.001);
    const float2 offset2 = float2(0.001, 0.0);
  
    float depth1 = DepthOnlyTex.Sample(g_LinearClampSampler, texcoords + offset1).r;
    float depth2 = DepthOnlyTex.Sample(g_LinearClampSampler, texcoords + offset2).r;
  
    float3 p1 = float3(offset1, depth1 - depth);
    float3 p2 = float3(offset2, depth2 - depth);
  
    float3 normal = cross(p1, p2);
    normal.z = -normal.z;
  
    return normalize(normal);
}

float4 main(PS_IN input) : SV_TARGET
{
    const float total_strength = 1.0;
    const float base = 0.2;
  
    const float area = 0.0075;
    const float falloff = 0.000001;
  
    const float radius = 0.0002;
  
    const int samples = 16;
    float3 sample_sphere[samples] =
    {
        float3(0.5381, 0.1856, -0.4319), float3(0.1379, 0.2486, 0.4430),
      float3(0.3371, 0.5679, -0.0057), float3(-0.6999, -0.0451, -0.0019),
      float3(0.0689, -0.1598, -0.8547), float3(0.0560, 0.0069, -0.1843),
      float3(-0.0146, 0.1402, 0.0762), float3(0.0100, -0.1924, -0.0344),
      float3(-0.3577, -0.5301, -0.4358), float3(-0.3169, 0.1063, 0.0158),
      float3(0.0103, -0.5869, 0.0046), float3(-0.0897, -0.4940, 0.3287),
      float3(0.7119, -0.0154, -0.0918), float3(-0.0533, 0.0596, -0.5411),
      float3(0.0352, -0.0631, 0.5460), float3(-0.4776, 0.2847, -0.0271)
    };
  
    float3 random = g_NoiseTex.Sample(g_PointSampler, input.vUV * 4.0).xyz;
    
    float depth = DepthOnlyTex.Sample(g_LinearClampSampler, input.vUV).r;
 
    float3 position = float3(input.vUV, depth);
    float3 normal = normal_from_depth(depth, input.vUV);
  
    float radius_depth = radius / depth;
    float occlusion = 0.0;
    for (int i = 0; i < samples; i++)
    {
        float3 ray = radius_depth * reflect(sample_sphere[i], random);
        float3 hemi_ray = position + sign(dot(ray, normal)) * ray;
    
        float occ_depth = DepthOnlyTex.Sample(g_LinearClampSampler, saturate(hemi_ray.xy)).r;
        float difference = depth - occ_depth;
    
        occlusion += step(falloff, difference) * (1.0 - smoothstep(falloff, area, difference));
    }
  
    float ao = 1.0 - total_strength * occlusion * (1.0 / samples);
    ao = saturate(ao + base);
  
    return float4(ao, ao, ao, 1.f);
}

