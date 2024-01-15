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
};

// ==================
// 상수버퍼 대응 구조체
// ==================
struct tTransform
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
struct tMtrlData
{
    Vec4 vAmb;  // 표면이 빛을 얼마나 받으냐에 따라 색을 결정하는 값
    Vec4 vDiff; // 조명과 상관없이 물체 자체가 갖고있는 색상
    Vec4 vSpec; // 시점으로부터 반사되는 빛이 얼마나 받느냐에 따라 결정되는 값
    Vec4 vEmv;
};

struct tMtrlConst
{
    tMtrlData mtrl; // float shininess; // 빛이 얼마나 집중 될지에 사용하는 값

    int arrInt[4];
    float arrFloat[4];
    Vec2 arrVec2[4];
    Vec4 arrVec4[4];
    Matrix arrMat[4];

    int bTex[TEX_PARAM::END - TEX_PARAM::TEX_0];

    int padding[2];
};

struct tGlobalData
{
    Vec2 g_RenderResolution;
    float g_dt;
    float g_time;

    UINT Light2DCount;
    UINT Light3DCount;
    float Bloom_Threshold;
    float Bloom_Strength;

    Vec4 pickingColor;

    Vec3 eyeWorld;
    int UseTexture;

    int UseRim;
    Vec3 rimColor;
    float rimPower;

    float NormalLineScale;
    int DrawAsWireFrame;

    float padding;
};

struct tAnimData2D
{
    Vec2 vLeftTop;
    Vec2 vSliceSize;
    Vec2 vBackGround;
    Vec2 vOffset;
    int UseAnim2D;
    int UseBackGround;
    Vec2 padding;
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

extern tTransform g_Transform;
extern tGlobalData g_Global;
