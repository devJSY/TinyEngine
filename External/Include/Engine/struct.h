
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

struct FTask
{
    TASK_TYPE Type;
    UINT_PTR Param_1;
    UINT_PTR Param_2;
};

struct tMaterialData
{
    Vec3 ambient;       // 조명과 상관없이 물체 자체가 갖고있는 색상
    float shininess;    // 빛이 얼마나 집중 될지에 사용하는 값
    Vec3 diffuse;       // 표면이 빛을 얼마나 받으냐에 따라 색을 결정하는 값
    float dummy1;                     
    Vec3 specular;      // 시점으로부터 반사되는 빛이 얼마나 받느냐에 따라 결정되는 값
    float dummy2;                     
};

struct tLightData
{
    Vec3 strength;
    float fallOffStart;    
    Vec3 direction;
    float fallOffEnd;
    Vec3 position;
    float spotPower;

    // 물체는 조명으로부터 거리에따라 빛을 받는양이 달라짐
    // fallOffStart, fallOffEnd 옵션값으로 표현
};

struct tGlobal
{   
    tLightData DirLight;       // 태양과 같이 아주 멀리있는 광원
    tLightData PointLight;     // 한 점으로 부터 여러방향으로 퍼져 나가는 광원
    tLightData SpotLight;      // 빛이 방향을 갖고있어 빛의 중심으로부터 가장자리로 갈수록 어두워지는 광원

    Vec4 eyeWorld;

    float NormalLineScale;
    Vec3 padd;
};


extern tTransform g_Transform;
extern tGlobal g_Global;
