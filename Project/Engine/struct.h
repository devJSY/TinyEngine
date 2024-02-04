#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
    Vec3 vPos;    // 정점의 좌표
    Vec3 vNormal; // 정점의 좌표
    Vec4 vColor;  // 정점의 색상 정보
    Vec2 vUV;     // UV 좌표계 of Texture Coordinate
};

struct tDebugShapeInfo
{
    DEBUG_SHAPE eShape;

    Vec3 vWorldPos;
    Vec3 vWorldScale;
    Vec3 vWorldRot;
    Matrix matWorld;

    Vec3 vColor;
    float fLifeTime;
    float fDuration;
    bool bDepthTest;
};

struct tLightInfo
{
    Vec4 vColor;    // 빛의 색상	- 광원의 순수 색상
    Vec4 vSpecular; // 반사광	- 광원이 물체 표변에 반사된 색상
    Vec4 vAmbient;  // 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛

    Vec3 vWorldPos; // 광원의 위치
    Vec3 vWorldDir; // 광원이 향하는 방향
    float fRadius;  // 광원의 영향 반경, 거리 정보
    float fAngle;   // 광원의 범위 각도

    int LightType; // 광원 타입

    // 물체는 조명으로부터 거리에따라 빛을 받는양이 달라짐
    // fallOffStart, fallOffEnd 옵션값으로 표현
    float fallOffStart;
    float fallOffEnd;
    float spotPower;

    Vec4 vRadiance; // Strength
};

// ==================
// 상수버퍼 대응 구조체
// ==================
__declspec(align(16)) struct tTransform
{
    Matrix matWorld;
    Matrix matWorldInv;
    Matrix matWorldInvTranspose;

    Matrix matView;
    Matrix matViewInv;

    Matrix matProj;
    Matrix matProjInv;

    Matrix matWV;
    Matrix matWVP;
};

// Material 계수
__declspec(align(16)) struct tMtrlData
{
    Vec4 vAmb;  // 표면이 빛을 얼마나 받으냐에 따라 색을 결정하는 값
    Vec4 vDiff; // 조명과 상관없이 물체 자체가 갖고있는 색상
    Vec4 vSpec; // 시점으로부터 반사되는 빛이 얼마나 받느냐에 따라 결정되는 값
    Vec4 vEmv;
};

__declspec(align(16)) struct tMtrlConst
{
    tMtrlData mtrl; // float shininess; // 빛이 얼마나 집중 될지에 사용하는 값

    int arrInt[4];
    float arrFloat[4];
    Vec2 arrVec2[4];
    Vec4 arrVec4[4];
    Matrix arrMat[4];

    int bTex[TEX_PARAM::END];
};

__declspec(align(16)) struct tGlobalData
{
    Vec2 g_RenderResolution;
    Vec2 g_NoiseTexResolution;
    float g_dt;
    float g_time;

    UINT Light2DCount;
    UINT Light3DCount;

    Vec4 pickingColor;

    Vec3 eyeWorld;
    int UseTexture;

    int UseRim;
    Vec3 rimColor;
    float rimPower;

    float NormalLineScale;
    int DrawAsWireFrame;
};

__declspec(align(16)) struct tAnimData2D
{
    Vec2 vLeftTop;
    Vec2 vSliceSize;
    Vec2 vBackGround;
    Vec2 vOffset;
    int UseAnim2D;
    int UseBackGround;
};

struct tMeshData
{
    std::vector<Vtx> vertices;
    std::vector<UINT> indices;

    string AmbientTextureFilename;
    string AoTextureFilename; // Ambient Occlusion
    string NormalTextureFilename;
    string HeightTextureFilename;
    string MetallicTextureFilename;
    string RoughnessTextureFilename;
    string EmissiveTextureFilename;

    string RelativeTextureFilePath; // 전체경로 = 절대경로/상대경로/파일이름 - 상대경로에 해당되는 문자열
};

struct tPixel
{
    BYTE r, g, b, a;
};

__declspec(align(16)) struct tParticle
{
    Vec4 vLocalPos;       // 로컬 위치
    Vec4 vWorldPos;       // 윌드 위치
    Vec4 vWorldRotation;  // 회전값
    Vec4 vWorldInitScale; // 초기 크기
    Vec4 vWorldScale;     // 크기
    Vec4 vVelocity;       // 속도
    Vec4 vColor;          // 색상
    Vec4 vForce;          // 입자에 적용된 누적 힘 총량
    Vec4 vNoiseForce;     // NoiseForce 모듈로 인한 랜덤 힘

    float NoiseForceTime; // NoiseForce 를 세팅받은 시간
    float NormalizeAge;   // Age 를 Life 기준으로 정규화한 값
    float Mass;           // 질량
    float Age;            // 현재 나이
    float Life;           // 수명
    int Active;           // 활성화, 비활성화 여부
};

__declspec(align(16)) struct tParticleModule
{
    // Sapwn 모듈
    Vec4 vSpawnColor;    // 초기 컬러
    Vec4 vSpawnMinScale; // 초기 최소 크기
    Vec4 vSpawnMaxScale; // 초기 최대 크기

    float MinLife;       // 최소 수명
    float MaxLife;       // 최대 수명
    float MinMass;       // 최소 질량
    float MaxMass;       // 최대 질량
    int SpawnRate;       // 초당 생성 개수
    int SpaceType;       // 좌표계(0 : LocalSpace, 1 : WorldSpace)
    int SpawnShape;      // 스폰 범위(0 : Sphere, 1 : Box)
    float Radius;        // SpawnShape 가 Sphere 인 경우, 반지름 길이
    Vec4 vSpawnBoxScale; // SpawnShape 가 Box 인 경우, Box 의 크기

    // Scale
    Vec4 vScaleRatio;

    // Add Velocity
    int AddVelocityType; // 0 : From Center, 1: To Center, 2: Fix Direction
    float MinSpeed;
    float MaxSpeed;
    float FixedAngle;     // 해당 방향에서 랜덤범위 각도
    Vec4 vFixedDirection; // 지정 방향

    // Noise Force
    float NoiseForceScale;
    float NoiseForceTerm;

    // Render
    int VelocityAlignment; // 1 : On, 0 : Off
    int AlphaBasedLife;    // 0 : off, 1 : NormalizedAge, 2: Max Age
    float AlphaMaxAge;

    // Module On / Off
    int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
};

__declspec(align(16)) struct tSpawnCount
{
    int SpawnCount;
};

extern tTransform g_Transform;
extern tGlobalData g_Global;
