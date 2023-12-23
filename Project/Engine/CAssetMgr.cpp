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
    vector<Vec2> texcoords; // ÅØ½ºÃç ÁÂÇ¥

    const float scale = 1.0f;

    // ¾Õ¸é
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
        0, 1, 2, 0, 2, 3, // ¾Õ¸é
    };

    return tuple{vertices, indices};
}

auto CAssetMgr::MakeBox()
{
    vector<Vec3> positions;
    vector<Vec3> normals;
    vector<Vec4> colors;
    vector<Vec2> texcoords; // ÅØ½ºÃç ÁÂÇ¥

    // À­¸é
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

    // ¾Æ·§¸é
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

    // ¾Õ¸é
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

    // µÞ¸é
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

    // ¿ÞÂÊ
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

    // ¿À¸¥ÂÊ
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
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
    };

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

        // Àü¿ªº¯¼ö¿¡ »ï°¢Çü À§Ä¡ ¼³Á¤
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

        // ÀÎµ¦½º
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

        // Áß½É Á¡
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

    // =========================
    // Shader
    // =========================
    CGraphicsShader* pShader = nullptr;

    pShader = new CGraphicsShader;
    pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
    pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

    pShader->SetRSType(RS_TYPE::CULL_NONE);
    pShader->SetDSType(DS_TYPE::LESS);
    pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

    AddAsset(L"Std2DShader", pShader);

    // ======================
    // Texture
    // ======================
    CTexture* pTex = Load<CTexture>(L"PlayerTexture", L"texture//MAP//BigMap.bmp");
    if (nullptr != pTex)
    {
        pTex->UpdateData(0);
    }

    // ======================
    // Material
    // ======================
    tMaterial material;
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
