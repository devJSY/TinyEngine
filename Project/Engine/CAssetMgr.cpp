#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

CAssetMgr::CAssetMgr()
{
}

CAssetMgr::~CAssetMgr()
{
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
    {
        for (auto pair : m_mapAsset[i])
        {
            if (nullptr != pair.second)
            {
                delete pair.second;
            }
        }

        m_mapAsset[i].clear();
    }
}

auto CAssetMgr::MakeSquare()
{
    vector<Vec3> positions;
    vector<Vec3> normals;
    vector<Vec4> colors;
    vector<Vec2> texcoords; // 텍스춰 좌표

    const float scale = 1.0f;

    // 앞면
    positions.push_back(Vec3(-1.0f, 1.0f, 0.0f) * scale);
    positions.push_back(Vec3(1.0f, 1.0f, 0.0f) * scale);
    positions.push_back(Vec3(1.0f, -1.0f, 0.0f) * scale);
    positions.push_back(Vec3(-1.0f, -1.0f, 0.0f) * scale);
    colors.push_back(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));

    // Texture Coordinates (Direct3D 9)
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    vector<Vtx> vertices;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vColor = colors[i];
        v.vNormal = normals[i];
        v.vUV = texcoords[i];

        vertices.push_back(v);
    }

    vector<UINT> indices = {
        0, 1, 2, 0, 2, 3, // 앞면
    };

    return tuple{vertices, indices};
}

auto CAssetMgr::MakeBox()
{
    vector<Vec3> positions;
    vector<Vec3> normals;
    vector<Vec4> colors;
    vector<Vec2> texcoords; // 텍스춰 좌표

    // 윗면
    positions.push_back(Vec3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vec3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vec3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vec3(1.0f, 1.0f, -1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 아랫면
    positions.push_back(Vec3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vec3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vec3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vec3(-1.0f, -1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vec3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 앞면
    positions.push_back(Vec3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vec3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vec3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vec3(1.0f, -1.0f, -1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 뒷면
    positions.push_back(Vec3(-1.0f, -1.0f, 1.0f));
    positions.push_back(Vec3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vec3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vec3(-1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 왼쪽
    positions.push_back(Vec3(-1.0f, -1.0f, 1.0f));
    positions.push_back(Vec3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vec3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vec3(-1.0f, -1.0f, -1.0f));
    colors.push_back(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    // 오른쪽
    positions.push_back(Vec3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vec3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vec3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vec3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vec2(0.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 0.0f));
    texcoords.push_back(Vec2(1.0f, 1.0f));
    texcoords.push_back(Vec2(0.0f, 1.0f));

    vector<Vtx> vertices;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vtx v;
        v.vPos = positions[i];
        v.vNormal = normals[i];
        v.vColor = colors[i];
        v.vUV = texcoords[i];

        vertices.push_back(v);
    }

    vector<UINT> indices = {
        0,  1,  2,  0,  2,  3,  // 윗면
        4,  5,  6,  4,  6,  7,  // 아랫면
        8,  9,  10, 8,  10, 11, // 앞면
        12, 13, 14, 12, 14, 15, // 뒷면
        16, 17, 18, 16, 18, 19, // 왼쪽
        20, 21, 22, 20, 22, 23  // 오른쪽
    };

    return tuple{vertices, indices};
}

auto CAssetMgr::MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices)
{
    // Texture 좌표계때문에 (numSlices + 1) x 2 개의 버텍스 사용
    const float dTheta = -XM_2PI / float(numSlices);

    vector<Vtx> vertices;

    // 옆면의 바닥 버텍스들 (인덱스 0 이상 numSlices 미만)
    for (int i = 0; i <= numSlices; i++)
    {
        Vtx v;
        v.vPos = Vec3::Transform(Vec3(bottomRadius, -0.5f * height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));

        v.vNormal = v.vPos - Vec3(0.0f, -0.5f * height, 0.0f);
        v.vNormal.Normalize();
        v.vUV = Vec2(float(i) / numSlices, 1.0f);

        vertices.push_back(v);
    }

    // 옆면의 맨 위 버텍스들 (인덱스 numSlices 이상 2 * numSlices 미만)
    for (int i = 0; i <= numSlices; i++)
    {
        Vtx v;
        v.vPos = Vec3::Transform(Vec3(topRadius, 0.5f * height, 0.0f), Matrix::CreateRotationY(dTheta * float(i)));
        v.vNormal = v.vPos - Vec3(0.0f, 0.5f * height, 0.0f);
        v.vNormal.Normalize();
        v.vUV = Vec2(float(i) / numSlices, 0.0f);

        vertices.push_back(v);
    }

    vector<UINT> indices;

    for (int i = 0; i < numSlices; i++)
    {
        indices.push_back(i);
        indices.push_back(i + numSlices + 1);
        indices.push_back(i + 1 + numSlices + 1);

        indices.push_back(i);
        indices.push_back(i + 1 + numSlices + 1);
        indices.push_back(i + 1);
    }

    return tuple{vertices, indices};
}

auto CAssetMgr::MakeSphere(const float radius, const int numSlices, const int numStacks)
{
    // 참고: OpenGL Sphere
    // http://www.songho.ca/opengl/gl_sphere.html
    // Texture 좌표계때문에 (numSlices + 1) 개의 버텍스 사용 (마지막에 닫아주는
    // 버텍스가 중복) Stack은 y 위쪽 방향으로 쌓아가는 방식

    const float dTheta = -XM_2PI / float(numSlices);
    const float dPhi = -XM_PI / float(numStacks);

    vector<Vtx> vertices;

    for (int j = 0; j <= numStacks; j++)
    {
        // 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
        Vec3 stackStartPoint = Vec3::Transform(Vec3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));

        for (int i = 0; i <= numSlices; i++)
        {
            Vtx v;

            // 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
            v.vPos = Vec3::Transform(stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

            v.vNormal = v.vPos; // 원점이 구의 중심
            v.vNormal.Normalize();
            v.vUV = Vec2(float(i) / numSlices, 1.0f - float(j) / numStacks);

            vertices.push_back(v);
        }
    }

    vector<UINT> indices;

    for (int j = 0; j < numStacks; j++)
    {
        const int offset = (numSlices + 1) * j;

        for (int i = 0; i < numSlices; i++)
        {
            indices.push_back(offset + i);
            indices.push_back(offset + i + numSlices + 1);
            indices.push_back(offset + i + 1 + numSlices + 1);

            indices.push_back(offset + i);
            indices.push_back(offset + i + 1 + numSlices + 1);
            indices.push_back(offset + i + 1);
        }
    }

    return tuple{vertices, indices};
}

auto CAssetMgr::MakeTetrahedron()
{
    // Regular Tetrahedron
    // https://mathworld.wolfram.com/RegularTetrahedron.html

    const float a = 1.0f;
    const float x = sqrt(3.0f) / 3.0f * a;
    const float d = sqrt(3.0f) / 6.0f * a; // = x / 2
    const float h = sqrt(6.0f) / 3.0f * a;

    vector<Vec3> points = {{0.0f, x, 0.0f}, {-0.5f * a, -d, 0.0f}, {+0.5f * a, -d, 0.0f}, {0.0f, 0.0f, h}};

    Vec3 center = Vec3(0.0f);

    for (int i = 0; i < 4; i++)
    {
        center += points[i];
    }
    center /= 4.0f;

    for (int i = 0; i < 4; i++)
    {
        points[i] -= center;
    }

    vector<Vtx> vertices;

    for (int i = 0; i < points.size(); i++)
    {
        Vtx v;
        v.vPos = points[i];
        v.vNormal = v.vPos; // 중심이 원점
        v.vNormal.Normalize();

        vertices.push_back(v);
    }

    vector<UINT> indices;

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    return tuple{vertices, indices};
}

auto CAssetMgr::MakeIcosahedron()
{
    // Luna DX12 교재 참고
    // 등20면체
    // https://mathworld.wolfram.com/Isohedron.html

    const float X = 0.525731f;
    const float Z = 0.850651f;

    vector<Vec3> pos = {Vec3(-X, 0.0f, Z), Vec3(X, 0.0f, Z),  Vec3(-X, 0.0f, -Z), Vec3(X, 0.0f, -Z),
                        Vec3(0.0f, Z, X),  Vec3(0.0f, Z, -X), Vec3(0.0f, -Z, X),  Vec3(0.0f, -Z, -X),
                        Vec3(Z, X, 0.0f),  Vec3(-Z, X, 0.0f), Vec3(Z, -X, 0.0f),  Vec3(-Z, -X, 0.0f)};

    vector<Vtx> vertices;

    for (size_t i = 0; i < pos.size(); i++)
    {
        Vtx v;
        v.vPos = pos[i];
        v.vNormal = v.vPos;
        v.vNormal.Normalize();

        vertices.push_back(v);
    }

    vector<UINT> indices;

    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(9);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(9);
    indices.push_back(8);
    indices.push_back(5);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(8);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(10);
    indices.push_back(8);
    indices.push_back(10);
    indices.push_back(3);
    indices.push_back(8);
    indices.push_back(8);
    indices.push_back(3);
    indices.push_back(5);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(5);
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(10);
    indices.push_back(7);
    indices.push_back(10);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(11);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(0);
    indices.push_back(11);
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(10);
    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(11);
    indices.push_back(0);
    indices.push_back(9);
    indices.push_back(2);
    indices.push_back(11);
    indices.push_back(9);
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(9);
    indices.push_back(11);
    indices.push_back(2);
    indices.push_back(7);

    return tuple{vertices, indices};
}

void CAssetMgr::init()
{
    // =========================
    // Mesh
    // =========================

    // Rect
    {
        CMesh* pMesh = nullptr;

        // 전역변수에 삼각형 위치 설정
        //   0(Red)-- 1(Blue)
        //    |   \   |
        //   3(G)---- 2(Magenta)
        Vtx arrVtx[4] = {};

        arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
        arrVtx[0].vNormal = Vec3(0.f, 0.f, -1.f);
        arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
        arrVtx[0].vUV = Vec2(0.f, 0.f);

        arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
        arrVtx[1].vNormal = Vec3(0.f, 0.f, -1.f);
        arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
        arrVtx[1].vUV = Vec2(1.f, 0.f);

        arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
        arrVtx[2].vNormal = Vec3(0.f, 0.f, -1.f);
        arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
        arrVtx[2].vUV = Vec2(1.f, 1.f);

        arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
        arrVtx[3].vNormal = Vec3(0.f, 0.f, -1.f);
        arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
        arrVtx[3].vUV = Vec2(0.f, 1.f);

        // 인덱스
        UINT arrIdx[6] = {};
        arrIdx[0] = 0;
        arrIdx[1] = 1;
        arrIdx[2] = 2;

        arrIdx[3] = 0;
        arrIdx[4] = 2;
        arrIdx[5] = 3;

        pMesh = new CMesh;
        pMesh->Create(arrVtx, 4, arrIdx, 6);
        AddAsset(L"RectMesh", pMesh);
    }

    // Circle
    {
        vector<Vtx> vecVtx;
        vector<UINT> vecIdx;
        Vtx v;

        // 중심 점
        v.vPos = Vec3(0.f, 0.f, 0.f);
        v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
        v.vUV = Vec2(0.5f, 0.5f);
        vecVtx.push_back(v);

        UINT iSlice = 40;
        float fTheta = 0.f;
        float fRadius = 0.5f;

        for (UINT i = 0; i <= iSlice; ++i)
        {
            fTheta = (XM_2PI / iSlice) * i;

            v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
            v.vNormal = Vec3(0.f, 0.f, -1.f);
            v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
            v.vUV = Vec2(0.f, 0.f);

            vecVtx.push_back(v);
        }

        for (UINT i = 0; i < iSlice; ++i)
        {
            vecIdx.push_back(0);
            vecIdx.push_back(i + 2);
            vecIdx.push_back(i + 1);
        }

        CMesh* pMesh = new CMesh;
        pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
        AddAsset(L"CircleMesh", pMesh);
    }

    // Square
    {
        auto [vertices, indices] = MakeSquare();

        CMesh* pMesh = new CMesh;
        pMesh->Create(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
        AddAsset(L"SquareMesh", pMesh);
    }

    // Box
    {
        auto [vertices, indices] = MakeBox();

        CMesh* pMesh = new CMesh;
        pMesh->Create(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
        AddAsset(L"BoxMesh", pMesh);
    }

    // Cylinder
    {
        auto [vertices, indices] = MakeCylinder(1, 1, 1, 100);
        CMesh* pMesh = new CMesh;
        pMesh->Create(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
        AddAsset(L"CylinderMesh", pMesh);
    }

    // Sphere
    {
        auto [vertices, indices] = MakeSphere(1, 25, 25);

        CMesh* pMesh = new CMesh;
        pMesh->Create(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
        AddAsset(L"SphereMesh", pMesh);
    }

    // Tetrahedron
    {
        auto [vertices, indices] = MakeTetrahedron();
        CMesh* pMesh = new CMesh;
        pMesh->Create(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
        AddAsset(L"TetrahedronMesh", pMesh);
    }

    // Icosahedron
    {
        auto [vertices, indices] = MakeIcosahedron();
        CMesh* pMesh = new CMesh;
        pMesh->Create(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
        AddAsset(L"IcosahedronMesh", pMesh);
    }

    // =========================
    // Shader
    // =========================
    {
        CGraphicsShader* pShader = nullptr;

        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
        pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

        pShader->SetRSType(RS_TYPE::CULL_NONE);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        AddAsset(L"Std2DShader", pShader);
    }

    {
        CGraphicsShader* pShader = nullptr;

        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\BlinnPhongVS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\BlinnPhongPS.hlsl", "main");

        pShader->SetRSType(RS_TYPE::CULL_BACK);
        pShader->SetDSType(DS_TYPE::LESS);
        pShader->SetBSType(BS_TYPE::DEFAULT);

        AddAsset(L"BlinnPhong", pShader);
    }

    {
        CGraphicsShader* pShader = nullptr;

        pShader = new CGraphicsShader;
        pShader->CreateVertexShader(L"shader\\NormalLineVS.hlsl", "main");
        pShader->CreateGeometryShader(L"shader\\NormalLineGS.hlsl", "main");
        pShader->CreatePixelShader(L"shader\\NormalLinePS.hlsl", "main");

        pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

        AddAsset(L"NormalLine", pShader);
    }

    // ======================
    // Texture
    // ======================
    CTexture* pTex = Load<CTexture>(L"PlayerTexture", L"texture//MAP//BigMap.bmp");
    if (nullptr != pTex)
    {
        pTex->UpdateData(0);
    }

    CTexture* pTex2 = Load<CTexture>(L"earth", L"texture//earth.jpg");
    if (nullptr != pTex2)
    {
        pTex2->UpdateData(1);
    }

    CTexture* pTex3 = Load<CTexture>(L"wall", L"texture//wall.jpg");
    if (nullptr != pTex3)
    {
        pTex3->UpdateData(2);
    }

    // ======================
    // Material
    // ======================
    tMaterialData material;
    material.shininess = 1.f;
    material.ambient = Vec3(1.0f, 0.0f, 0.0f);
    material.diffuse = Vec3(0.5f);
    material.specular = Vec3(0.5f);

    CMaterial* Rmat = new CMaterial;
    Rmat->Create(material);

    material.ambient = Vec3(0.0f, 1.0f, 0.0f);
    CMaterial* Gmat = new CMaterial;
    Gmat->Create(material);

    material.ambient = Vec3(0.0f, 0.0f, 1.0f);
    CMaterial* Bmat = new CMaterial;
    Bmat->Create(material);

    AddAsset(L"RedMaterial", Rmat);
    AddAsset(L"GreenMaterial", Gmat);
    AddAsset(L"BlueMaterial", Bmat);
}
