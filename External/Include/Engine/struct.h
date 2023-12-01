#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
    Vec3 vPos;   // 정점의 좌표
    Vec4 vColor; // 정점의 색상 정보
    Vec2 vUV;    // UV 좌표계 of Texture Coordinate
};