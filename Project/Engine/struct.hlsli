#ifndef _STRUCT
#define _STRUCT

struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct PS_IN
{
    float4 vPosProj : SV_Position;
    float3 vPosWorld : POSITION;
    float3 normalWorld : NORMAL;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct tLightInfo
{
    float4 vColor; // 빛의 색상	- 광원의 순수 색상
    float4 vSpecular; // 반사광	- 광원이 물체 표변에 반사된 색상
    float4 vAmbient; // 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛

    float3 vWorldPos; // 광원의 위치
    float3 vWorldDir; // 광원이 향하는 방향
    float fRadius; // 광원의 영향 반경, 거리 정보
    float fAngle; // 광원의 범위 각도

    int LightType; // 광원 타입

    // 물체는 조명으로부터 거리에따라 빛을 받는양이 달라짐
    // fallOffStart, fallOffEnd 옵션값으로 표현
    float fallOffStart;
    float fallOffEnd;
    float spotPower;
};

#endif