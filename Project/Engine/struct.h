#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
    Vec3 vPos;    // 정점의 좌표
    Vec3 vNormal; // 정점의 좌표
    Vec4 vColor;  // 정점의 색상 정보
    Vec2 vUV;     // UV 좌표계 of Texture Coordinate
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
    tMtrlData mtrl;

    int iArr[4];
    float fArr[4]; // float shininess; // 빛이 얼마나 집중 될지에 사용하는 값
    Vec2 v2Arr[4];
    Vec4 v4Arr[4];
    Matrix matArr[4];

    int arrTex[(UINT)TEX_PARAM::END];

    int arrAnimData[2]; // 3D Animation 정보
};

struct tLightData
{
    Vec3 strength; // Color
    float fallOffStart;
    Vec3 direction;
    float fallOffEnd;
    Vec3 position;
    float spotPower;

    // 물체는 조명으로부터 거리에따라 빛을 받는양이 달라짐
    // fallOffStart, fallOffEnd 옵션값으로 표현

    UINT LightType;
    Vec3 padd;
};

struct tGlobal
{
    tLightData Lights[MAX_LIGHTS];

    Vec3 eyeWorld;
    int UseTexture;

    Vec3 rimColor;
    float rimPower;

    float NormalLineScale;

    int UseRim;
    bool DrawAsWireFrame;
    int padd;
};

struct tMeshData
{
    std::vector<Vtx> vertices;
    std::vector<UINT> indices;

    std::string AlbedoTextureFilename;
    std::string AoTextureFilename; // Ambient Occlusion
    std::string NormalTextureFilename;
    std::string HeightTextureFilename;
    std::string MetallicTextureFilename;
    std::string RoughnessTextureFilename;
    std::string EmissiveTextureFilename;

    std::string RelativeTextureFilePath; // 전체경로 = 절대경로/상대경로/파일이름 - 상대경로에 해당되는 문자열
};

extern tTransform g_Transform;
extern tGlobal g_Global;
