#include "global.hlsli"

// https://www.shadertoy.com/view/wdVXWR

struct PS_Input
{
    float4 pos : SV_POSITION;
    float4 posWolrd : POSITION0;
    float4 center : POSITION1;
    float2 texCoord : TEXCOORD;
};

float3 fire_color(float x)
{
    return
        // red
        float3(1., 0., 0.) * x
        // yellow
        + float3(1., 1., 0.) * clamp(x - .5, 0., 1.)
        // white
        + float3(1., 1., 1.) * clamp(x - .7, 0., 1.);
}

Matrix rotate(float a, float3 v)
{
    float c = cos(a);
    float3 ci = (1. - c) * v;
    float3 s = sin(a) * v;

    return transpose(Matrix(
        ci.x * v.x + c, ci.x * v.y + s.z, ci.x * v.z - s.y, 0,
        ci.y * v.x - s.z, ci.y * v.y + c, ci.y * v.z + s.x, 0,
        ci.z * v.x + s.y, ci.z * v.y - s.x, ci.z * v.z + c, 0,
        0, 0, 0, 1
	));
}

// intersect ray with sphere to find
//  - the distance to the sphere
//  - and the point of intersection on the sphere
// http://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/
float intersect_ray_sphere(float3 origin, float3 direction, float3 center, float radius)
{
    float3 oc = origin - center;
    float a = dot(direction, direction);
    float b = 2. * dot(oc, direction);
    float c = dot(oc, oc) - radius * radius;
    float disc = b * b - 4. * a * c;
    if (disc < 0.)
    {
        // no intersection?
        return -1.;
    }
    else
    {
        return (-b - sqrt(disc)) / (2. * a);
    }
}

float4 main(PS_Input input) : SV_TARGET
{
    float3 eye = g_eyeWorld;
    float3 dir = normalize(input.posWolrd.xyz - eye);
    float3 sphere_pos = input.center.xyz;
    float radiusScale = 100;
    float currentTime = -g_time;
        
    float intensity = 0.;
    
    Matrix tex_mat = Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    Matrix wind_mat = Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    
    for (int i = 1; i < 10; ++i) // 나누기 때문에 i = 0 제외 
    {
        float dist = intersect_ray_sphere(eye, dir, sphere_pos, (0.4 - float(i) / 200.0) * radiusScale);

        if (dist > 0.0)
        {
            tex_mat = mul(tex_mat, rotate(radians(11.) * currentTime, normalize(float3(0.3, -0.7, 0.1))));
            wind_mat = mul(wind_mat, rotate(radians(25.) * currentTime, normalize(float3(1.0, 0.0, 0.0))));
            
            float3 hit_pos = eye + dir * dist;
            float3 normal = normalize(hit_pos - sphere_pos);
            normal = mul(float4(normal, 1.), mul(wind_mat, tex_mat)).xyz;
            
            float2 uv = float2(1.0 / float(i) * float2(atan2(normal.x, normal.z) / radians(90.0), normal.y));
            float alpha = 0.0;
            if (g_UseTexture && g_btex_0)
                alpha = g_tex_0.Sample(g_LinearWrapSampler, input.texCoord).r;
 
            intensity += step(1.0 - float(i) / 6.0, alpha) * 0.6 * alpha * max(0., dot(float3(0, 0, 1.), normal) + 1.0);
        }
    }
    
    float3 glow = fire_color(2.) * max(0., 1.3 - 1.8 * length(input.texCoord * 2.0 - 1.0));
    float3 color = fire_color(intensity);
    
    float4 output = float4(.09 * glow + 0.8 * color, 1.0);
    
    // Black Color Clip
    // RGB의 평균이 0.1보다 작은경우
    clip(dot(output.rgb, float3(1.0, 1.0, 1.0)) - 0.1);
    
    return output;
}
