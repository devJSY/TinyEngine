#ifndef _STRUCT
#define _STRUCT

struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosProj : SV_Position;
    float3 vPosWorld : POSITION;
    float3 normalWorld : NORMAL;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

// Α¶Έν
struct Light
{
    float3 strength;
    float fallOffStart;
    float3 direction;
    float fallOffEnd;
    float3 position;
    float spotPower;
    
    uint LightType;
    float3 padd;
};

#endif