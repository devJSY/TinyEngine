#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
    Vec3 vPos;      // 정점의 좌표
    Vec3 vNormal;      // 정점의 좌표
    Vec4 vColor;    // 정점의 색상 정보
    Vec2 vUV;       // UV 좌표계 of Texture Coordinate
};

// ==================
// 상수버퍼 대응 구조체
// ==================
struct tTransform
{
    Matrix matWorld;
    Matrix matWorldInv;

    Matrix matView;
    Matrix matViewInv;

    Matrix matProj;
    Matrix matProjInv;

    Matrix matWV;
    Matrix matWVP;
};

extern tTransform g_Transform;

struct FTask
{
    TASK_TYPE Type;
    UINT_PTR Param_1;
    UINT_PTR Param_2;
};
