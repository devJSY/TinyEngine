
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

struct tMaterial
{
    Vec3 ambient;
    float shininess;
    Vec3 diffuse;   
    float dummy1;                     
    Vec3 specular;
    float dummy2;                     
};

struct tLight
{
    Vec3 strength;
    float fallOffStart;
    Vec3 direction;
    float fallOffEnd;
    Vec3 position;
    float spotPower;
};

struct tGlobal
{
    tLight DirLight;
    tLight PointLight;
    tLight SpotLight;

    Vec4 eyeWorld;
};


extern tTransform g_Transform;
extern tGlobal g_Global;
