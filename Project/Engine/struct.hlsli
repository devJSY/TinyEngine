#ifndef _STRUCT
#define _STRUCT

struct VS_IN
{
    float3 vPos : POSITION;
  
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBitangent : BINORMAL;
    
    float4 vColor : COLOR;
    
    float2 vUV0 : TEXCOORD;
    float2 vUV1 : TEXCOORD;
    float2 vUV2 : TEXCOORD;
    float2 vUV3 : TEXCOORD;
    
    float4 vIndices : BLENDINDICES;
    float4 vWeights : BLENDWEIGHT;
        
    // Per Instance Data    
    row_major matrix matWorld : WORLD;
    row_major matrix matWorldInvTranspose : WORLDINVTRANSPOSE;
    row_major matrix matView : VIEW;
    row_major matrix matProj : PROJ;
    uint iRowIndex : ROWINDEX;
};

struct PS_IN
{
    float4 vPosProj : SV_Position;
    float3 vPosWorld : POSITION;
    
    float3 vNormalWorld : NORMAL;
    float3 vTangentWorld : TANGENT;
    float3 vBitangentWorld : BINORMAL;
    
    float4 vColor : COLOR;
    
    float2 vUV0 : TEXCOORD0;
    float2 vUV1 : TEXCOORD1;
    float2 vUV2 : TEXCOORD2;
    float2 vUV3 : TEXCOORD3;
};

struct tLightInfo
{
    float4 vRadiance; // 빛이 나가는 강도 
    
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
    
    row_major Matrix viewMat;
    row_major Matrix projMat;
    row_major Matrix invProj;
    
    int ShadowIndex;
    float HaloRadius;
    float HaloStrength;
    int LightPadd;
};

struct tTileInfo
{
    float2 vLeftTopUV;
    int bRender;
    int ImageIndex;
};

struct tParticle
{
    float4 vLocalPos; // 로컬 위치
    float4 vWorldPos; // 윌드 위치
    float4 vWorldRotation; // 회전값
    float4 vWorldInitScale; // 초기 크기
    float4 vWorldScale; // 크기
    float4 vVelocity; // 속도
    float4 vColor; // 색상
    float4 vForce; // 입자에 적용된 누적 힘 총량
    float4 vNoiseForce; // NoiseForce 모듈로 인한 랜덤 힘

    float NoiseForceTime; // NoiseForce 를 세팅받은 시간
    float NormalizeAge; // Age 를 Life 기준으로 정규화한 값
    float Mass; // 질량
    float Age; // 현재 나이
    float Life; // 수명
    int Active; // 활성화, 비활성화 여부
    
    int2 particlepadding;
};

struct tParticleModule
{
    // Sapwn 모듈
    float4 vSpawnColor; // 초기 컬러
    float4 vSpawnMinScale; // 초기 최소 크기
    float4 vSpawnMaxScale; // 초기 최대 크기

    float MinLife; // 최소 수명
    float MaxLife; // 최대 수명
    float MinMass; // 최소 질량
    float MaxMass; // 최대 질량
    int SpawnRate; // 초당 생성 개수
    int SpaceType; // 좌표계(0 : LocalSpace, 1 : WorldSpace
    int SpawnShape; // 스폰 범위(0 : Sphere, 1 : Box)
    float Radius; // SpawnShape 가 Sphere 인 경우, 반지름 길이
    float4 vSpawnBoxScale; // SpawnShape 가 Box 인 경우, Box 의 크기
        
    // Drag
    float DragTime; // 감속시키는데 걸리는 시간
    
    // Scale
    float4 vScaleRatio;

    // Add Velocity
    int AddVelocityType; // 0 : From Center, 1: To Center, 2: Fix Direction
    float MinSpeed;
    float MaxSpeed;
    float FixedAngle; // 해당 방향에서 랜덤범위 각도
    float4 vFixedDirection; // 지정 방향

    // Noise Force
    float NoiseForceScale;
    float NoiseForceTerm;
    
	// Render
    int VelocityAlignment; // 1 : On, 0 : Off
    int AlphaBasedLife; // 0 : off, 1 : NomrlizedAge, 2: Age
    float AlphaMaxAge;

    // Module On / Off
    int arrModuleCheck[7];
    
    float3 modulepadd;
};

struct tRaycastOut
{
    float2 vUV;
    float fDist;
    int success;
};

struct tSkinningInfo
{
    float3 vPos;
    float3 vTangent;
    float3 vBinormal;
    float3 vNormal;
};

#endif