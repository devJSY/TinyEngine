#include "pch.h"
#include "ShObjIdl_core.h" // FileDialog

#include "CDevice.h"

#include "CTaskMgr.h"
#include "CRenderMgr.h"
#include "CEditorMgr.h"
#include "CEngine.h"
#include "func.h"
#include "components.h"
#include <random>

// stb
extern "C"
{
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
}

void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
    tTask task = {};
    task.Type = TASK_TYPE::CREATE_OBJECT;
    task.Param_1 = (DWORD_PTR)_LayerIdx;
    task.Param_2 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject)
{
    tTask task = {};
    task.Type = TASK_TYPE::ADD_CHILD;
    task.Param_1 = (DWORD_PTR)_ParentObject;
    task.Param_2 = (DWORD_PTR)_ChildObject;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
    tTask task = {};
    task.Type = TASK_TYPE::DELETE_OBJECT;
    task.Param_1 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::WindowResize(int width, int height)
{
    tTask task;
    task.Type = TASK_TYPE::WINDOW_RESIZE;
    task.Param_1 = (INT_PTR)width;
    task.Param_2 = (INT_PTR)height;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::CloneGameObject(CGameObject* _OriginObj)
{
    tTask task;
    task.Type = TASK_TYPE::CLONE_OBJECT;
    task.Param_1 = (DWORD_PTR)_OriginObj;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugLine(const Matrix& _WorldMat, Vec3 _p1, Vec3 _p2, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::LINE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    wstring MeshName = L"LineMesh_";
    MeshName += std::to_wstring(_p1.x);
    MeshName += std::to_wstring(_p1.y);
    MeshName += std::to_wstring(_p1.z);
    MeshName += L"_";
    MeshName += std::to_wstring(_p2.x);
    MeshName += std::to_wstring(_p2.y);
    MeshName += std::to_wstring(_p2.z);

    Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MeshName);

    if (nullptr == pMesh)
    {
        // Line Mesh
        vector<Vec3> positions;
        vector<Vec3> colors;
        vector<Vec2> texcoords;

        tMeshData meshData;
        positions.push_back(_p1);
        positions.push_back(_p2);

        texcoords.push_back(Vec2(0.f, 0.f));
        texcoords.push_back(Vec2(0.f, 0.f));

        colors.push_back(_Color);
        colors.push_back(_Color);

        for (size_t i = 0; i < positions.size(); i++)
        {
            Vtx v;
            v.vPos = positions[i];
            v.vUV = texcoords[i];
            v.vColor = colors[i];
            v.vColor.w = 1.f;

            meshData.vertices.push_back(v);
        }

        meshData.indices.push_back(0);
        meshData.indices.push_back(1);

        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
        CAssetMgr::GetInst()->AddAsset(MeshName, pMesh);
    }

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugLine(Vec3 _vWorldPos, Vec3 _vDir, float _fLength, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::LINE;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fLength, _fLength, _fLength);
    info.vWorldRot = Vec3();

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    wstring MeshName = L"LineMesh_";
    MeshName += std::to_wstring(_vDir.x);
    MeshName += std::to_wstring(_vDir.y);
    MeshName += std::to_wstring(_vDir.z);
    MeshName += L"_";
    MeshName += std::to_wstring(_fLength);

    Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MeshName);

    if (nullptr == pMesh)
    {
        // Line Mesh
        vector<Vec3> positions;
        vector<Vec3> colors;
        vector<Vec2> texcoords;

        tMeshData meshData;
        positions.push_back(Vec3(0.f, 0.f, 0.f));
        positions.push_back(_vDir.Normalize());

        texcoords.push_back(Vec2(0.f, 0.f));
        texcoords.push_back(Vec2(0.f, 0.f));

        colors.push_back(_Color);
        colors.push_back(_Color);

        for (size_t i = 0; i < positions.size(); i++)
        {
            Vtx v;
            v.vPos = positions[i];
            v.vUV = texcoords[i];
            v.vColor = colors[i];
            v.vColor.w = 1.f;

            meshData.vertices.push_back(v);
        }

        meshData.indices.push_back(0);
        meshData.indices.push_back(1);

        Ptr<CMesh> pMesh = new CMesh(true);
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
        CAssetMgr::GetInst()->AddAsset(MeshName, pMesh);
    }

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::RECT;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::RECT;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = _vWorldScale;
    info.vWorldRot = _vWorldRot;

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CIRCLE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CIRCLE;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
    info.vWorldRot = Vec3(0.f, 0.f, 0.f);

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CROSS;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fScale, _fScale, _fScale);
    info.vWorldRot = Vec3(0.f, 0.f, 0.f);

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"CrosshairMesh");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugBox(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::BOX;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"WireBoxMesh");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugBox(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::BOX;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = _vWorldScale;
    info.vWorldRot = _vWorldRot;
    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"WireBoxMesh");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugSphere(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::SPHERE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"WireSphereMesh");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::SPHERE;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fRadius, _fRadius, _fRadius);
    info.vWorldRot = Vec3(0.f, 0.f, 0.f);

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"WireSphereMesh");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCone(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CONE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCapsule(const Matrix& _WorldMat, float _fRadius, float _HalfHeight, AXIS_TYPE _Axis, Vec3 _Color, bool _bDepthTest,
                                      float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CAPSULE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    wstring MeshName = L"WireCapsuleMesh_";
    MeshName += std::to_wstring(_fRadius);
    MeshName += L"_";
    MeshName += std::to_wstring(_HalfHeight);

    Matrix RotationMatrix = Matrix();
    switch (_Axis)
    {
    case AXIS_TYPE::X: {
        MeshName += L"_X";
        RotationMatrix = XMMatrixRotationZ(XM_PIDIV2);
    }
    break;
    case AXIS_TYPE::Y: {
        MeshName += L"_Y";
    }
    break;
    case AXIS_TYPE::Z: {
        MeshName += L"_Z";
        RotationMatrix = XMMatrixRotationX(XM_PIDIV2);
    }
    break;
    }

    Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MeshName);

    if (nullptr == pMesh)
    {
        auto meshData = CAssetMgr::GetInst()->MakeWireCapsule(_fRadius, _HalfHeight, 15);
        for (auto& vtx : meshData.vertices)
            vtx.vPos = Vector3::Transform(vtx.vPos, RotationMatrix);

        pMesh = new CMesh(true);
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
        CAssetMgr::GetInst()->AddAsset(MeshName, pMesh);
    }

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugPolygon(const Matrix& _WorldMat, Vec3 _Color, const vector<Vec2>& _Polygon, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::POLYGON;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    wstring MeshName = L"PolygonMesh_";
    for (size_t i = 0; i < _Polygon.size(); i++)
    {
        MeshName += std::to_wstring(_Polygon[i].x);
        MeshName += std::to_wstring(_Polygon[i].y);
        MeshName += L"_";
    }

    Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MeshName);

    if (nullptr == pMesh)
    {
        // Polygon Mesh
        vector<Vec3> positions;
        vector<Vec3> colors;
        vector<Vec3> normals;
        vector<Vec2> texcoords;

        for (size_t i = 0; i < _Polygon.size(); i++)
        {
            positions.push_back(Vec3(_Polygon[i].x, _Polygon[i].y, 0.f));
            colors.push_back(_Color);
            normals.push_back(Vec3(0.f, 0.f, -1.f));
            texcoords.push_back(Vec2(0.f, 0.f));
        }

        tMeshData meshData;

        for (UINT i = 0; i < _Polygon.size(); i++)
        {
            Vtx v;
            v.vPos = positions[i];
            v.vNormal = normals[i];
            v.vUV = texcoords[i];
            v.vColor = colors[i];
            v.vColor.w = 1.f;

            meshData.vertices.push_back(v);
        }

        for (UINT i = 0; i < _Polygon.size(); i++)
        {
            if (i == _Polygon.size() - 1)
            {
                meshData.indices.push_back(i);
                meshData.indices.push_back(0);
            }
            else
            {
                meshData.indices.push_back(i);
                meshData.indices.push_back(i + 1);
            }
        }

        pMesh = new CMesh(true);
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
        CAssetMgr::GetInst()->AddAsset(MeshName, pMesh);
    }

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugPolygon(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, const vector<Vec2>& _Polygon,
                                      bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::POLYGON;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = _vWorldScale;
    info.vWorldRot = _vWorldRot;

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    wstring MeshName = L"PolygonMesh_";
    for (size_t i = 0; i < _Polygon.size(); i++)
    {
        MeshName += std::to_wstring(_Polygon[i].x);
        MeshName += std::to_wstring(_Polygon[i].y);
        MeshName += L"_";
    }

    Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MeshName);

    if (nullptr == pMesh)
    {
        // Polygon Mesh
        vector<Vec3> positions;
        vector<Vec3> colors;
        vector<Vec3> normals;
        vector<Vec2> texcoords;

        for (size_t i = 0; i < _Polygon.size(); i++)
        {
            positions.push_back(Vec3(_Polygon[i].x, _Polygon[i].y, 0.f));
            colors.push_back(_Color);
            normals.push_back(Vec3(0.f, 0.f, -1.f));
            texcoords.push_back(Vec2(0.f, 0.f));
        }

        tMeshData meshData;

        for (UINT i = 0; i < _Polygon.size(); i++)
        {
            Vtx v;
            v.vPos = positions[i];
            v.vNormal = normals[i];
            v.vUV = texcoords[i];
            v.vColor = colors[i];
            v.vColor.w = 1.f;

            meshData.vertices.push_back(v);
        }

        for (UINT i = 0; i < _Polygon.size(); i++)
        {
            if (i == _Polygon.size() - 1)
            {
                meshData.indices.push_back(i);
                meshData.indices.push_back(0);
            }
            else
            {
                meshData.indices.push_back(i);
                meshData.indices.push_back(i + 1);
            }
        }

        pMesh = new CMesh(true);
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
        CAssetMgr::GetInst()->AddAsset(MeshName, pMesh);
    }

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugFrustum(const Vec3 WorldVertex[8], Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::FRUSTUM;
    info.matWorld = Matrix();
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    wstring MeshName = L"FrustumMesh_";
    for (UINT i = 0; i < 8; i++)
    {
        MeshName += std::to_wstring(WorldVertex[i].x);
        MeshName += std::to_wstring(WorldVertex[i].y);
        MeshName += std::to_wstring(WorldVertex[i].z);
        MeshName += L"_";
    }

    Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MeshName);

    if (nullptr == pMesh)
    {
        vector<Vec3> positions;
        vector<Vec3> colors;

        tMeshData meshData;

        for (UINT i = 0; i < 8; i++)
        {
            Vtx v;
            v.vPos = WorldVertex[i];
            v.vColor = _Color;

            meshData.vertices.push_back(v);
        }

        // Near
        meshData.indices.push_back(0);
        meshData.indices.push_back(1);
        meshData.indices.push_back(1);
        meshData.indices.push_back(2);
        meshData.indices.push_back(2);
        meshData.indices.push_back(3);
        meshData.indices.push_back(3);
        meshData.indices.push_back(0);

        // Far
        meshData.indices.push_back(4);
        meshData.indices.push_back(5);
        meshData.indices.push_back(5);
        meshData.indices.push_back(6);
        meshData.indices.push_back(6);
        meshData.indices.push_back(7);
        meshData.indices.push_back(7);
        meshData.indices.push_back(4);

        // Edge
        meshData.indices.push_back(0);
        meshData.indices.push_back(4);
        meshData.indices.push_back(1);
        meshData.indices.push_back(5);
        meshData.indices.push_back(2);
        meshData.indices.push_back(6);
        meshData.indices.push_back(3);
        meshData.indices.push_back(7);

        pMesh = new CMesh(true);
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());
        CAssetMgr::GetInst()->AddAsset(MeshName, pMesh);
    }

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::ScreenShot()
{
    tTask task = {};
    task.Type = TASK_TYPE::SCREENSHOT;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::MouseColorPicking(Vec2 MousePos)
{
    tTask task = {};
    task.Type = TASK_TYPE::MOUSE_COLOR_PICKING;
    task.Param_1 = (INT_PTR)MousePos.x;
    task.Param_2 = (INT_PTR)MousePos.y;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::MouseCollision2DPicking(Vec2 MousePos)
{
    tTask task = {};
    task.Type = TASK_TYPE::MOUSE_COLLISION2D_PICKING;
    task.Param_1 = (INT_PTR)MousePos.x;
    task.Param_2 = (INT_PTR)MousePos.y;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddComponent(CGameObject* _pObj, COMPONENT_TYPE _type)
{
    tTask task = {};
    task.Type = TASK_TYPE::ADD_COMPONENT;
    task.Param_1 = (INT_PTR)_pObj;
    task.Param_2 = (INT_PTR)_type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::RemoveComponent(CGameObject* _pObj, COMPONENT_TYPE _type)
{
    tTask task = {};
    task.Type = TASK_TYPE::REMOVE_COMPONENT;
    task.Param_1 = (INT_PTR)_pObj;
    task.Param_2 = (INT_PTR)_type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::RemoveScript(CGameObject* _pObj, CScript* _script)
{
    tTask task = {};
    task.Type = TASK_TYPE::REMOVE_SCRIPT;
    task.Param_1 = (INT_PTR)_pObj;
    task.Param_2 = (INT_PTR)_script;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::LayerChange(CGameObject* _pObj, int LayerIdx)
{
    tTask task = {};
    task.Type = TASK_TYPE::CHANGE_LAYER;
    task.Param_1 = (DWORD_PTR)_pObj;
    task.Param_2 = (INT_PTR)LayerIdx;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextState)
{
    tTask task = {};
    task.Type = TASK_TYPE::CHANGE_LEVEL;
    task.Param_1 = (DWORD_PTR)_NextLevel;
    task.Param_2 = (DWORD_PTR)_NextState;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::ChangeLevelState(CLevel* _NextLevel, LEVEL_STATE _NextState)
{
    tTask task = {};
    task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
    task.Param_1 = (DWORD_PTR)_NextLevel;
    task.Param_2 = (DWORD_PTR)_NextState;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DeleteAsset(ASSET_TYPE _type, Ptr<CAsset> _Asset)
{
    tTask task = {};
    task.Type = TASK_TYPE::DELETE_ASSET;
    task.Param_1 = (DWORD_PTR)_type;
    task.Param_2 = (DWORD_PTR)_Asset.Get();
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::Physics2D_Event(CGameObject* _pObj, Physics2D_EVENT_TYPE _Type)
{
    tTask task = {};
    task.Type = TASK_TYPE::PHYSICS2D_EVNET;
    task.Param_1 = (DWORD_PTR)_pObj;
    task.Param_2 = (DWORD_PTR)_Type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::Physics_Event(CGameObject* _pObj, Physics_EVENT_TYPE _Type)
{
    tTask task = {};
    task.Type = TASK_TYPE::PHYSICS_EVNET;
    task.Param_1 = (DWORD_PTR)_pObj;
    task.Param_2 = (DWORD_PTR)_Type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    if (nullptr != pSound)
    {
        pSound->Play(_Loop, _Volume, _Overlap);
    }
}

void GamePlayStatic::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
    static Ptr<CSound> CurBGM = nullptr;

    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    if (nullptr != pSound)
    {
        if (nullptr != CurBGM)
        {
            CurBGM->Stop();
        }

        pSound->Play(0, _Volume);
        CurBGM = pSound;
    }
}

int GetRandomInt(int _Min, int _Max)
{
    if (_Min > _Max)
        std::swap(_Min, _Max);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(_Min, _Max);
    return dist(mt);
}

float GetRandomfloat(float _Min, float _Max)
{
    if (_Min > _Max)
        std::swap(_Min, _Max);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(_Min, _Max);
    return floor(dist(mt)); // (숫자가 클 경우 정밀도 문제 발생 -> floor로 절삭)
}

float Lerp(float A, float B, float Alpha)
{
    return A * (1 - Alpha) + B * Alpha;
}

string ToString(const wstring& wstr)
{
    string str(wstr.length(), 0);
    std::transform(wstr.begin(), wstr.end(), str.begin(), [](wchar_t c) { return (char)c; });
    return str;
}

wstring ToWstring(const string& str)
{
    return wstring().assign(str.begin(), str.end());
}

string currentDateTime()
{
    // 현재 시간을 구하기
    std::time_t currentTime = std::time(nullptr);

    // 지역 시간 구조체
    std::tm localTime;

    // localtime_s 함수를 사용하여 안전하게 지역 시간 얻기
    localtime_s(&localTime, &currentTime);

    // 포맷 문자열
    const char* format = "%Y-%m-%d_%H-%M-%S";

    // 포맷에 맞게 문자열로 변환
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), format, &localTime);

    return buffer;
}

Vec4 HashIDToColor(int hash)
{
    int r = (hash >> 16) & 0xff;
    int g = (hash >> 8) & 0xff;
    int b = hash & 0xff;

    return Vec4(static_cast<float>(r / 255.f), static_cast<float>(g / 255.f), static_cast<float>(b / 255.f), 1.f);
}

void ReadImage(const std::string filename, std::vector<uint8_t>& image, int& width, int& height)
{
    int channels;

    unsigned char* img = stbi_load((ToString(CPathMgr::GetContentPath()) + filename).c_str(), &width, &height, &channels, 0);

    // 4채널로 만들어서 복사
    image.resize(width * height * 4);

    if (channels == 1)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            uint8_t g = img[i * channels + 0];
            for (size_t c = 0; c < 4; c++)
            {
                image[4 * i + c] = g;
            }
        }
    }
    else if (channels == 2)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            for (size_t c = 0; c < 2; c++)
            {
                image[4 * i + c] = img[i * channels + c];
            }
            image[4 * i + 2] = 255;
            image[4 * i + 3] = 255;
        }
    }
    else if (channels == 3)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                image[4 * i + c] = img[i * channels + c];
            }
            image[4 * i + 3] = 255;
        }
    }
    else if (channels == 4)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            for (size_t c = 0; c < 4; c++)
            {
                image[4 * i + c] = img[i * channels + c];
            }
        }
    }
    else
    {
        LOG(Error, "Cannot read %d channels", channels);
    }

    delete[] img;
}

ComPtr<ID3D11Texture2D> CreateStagingTexture(const int width, const int height, const std::vector<uint8_t>& image, const DXGI_FORMAT pixelFormat,
                                             const int mipLevels, const int arraySize)
{
    // 스테이징 텍스춰 만들기
    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = mipLevels;
    txtDesc.ArraySize = arraySize;
    txtDesc.Format = pixelFormat;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_STAGING;
    txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

    ComPtr<ID3D11Texture2D> stagingTexture;
    if (FAILED(DEVICE->CreateTexture2D(&txtDesc, NULL, stagingTexture.GetAddressOf())))
    {
        LOG(Error, "Create Failed Staging Texture");
    }

    // CPU에서 이미지 데이터 복사
    size_t pixelSize = sizeof(uint8_t) * 4;
    if (pixelFormat == DXGI_FORMAT_R16G16B16A16_FLOAT)
    {
        pixelSize = sizeof(uint16_t) * 4;
    }

    D3D11_MAPPED_SUBRESOURCE ms;
    CONTEXT->Map(stagingTexture.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);
    uint8_t* pData = (uint8_t*)ms.pData;
    for (UINT h = 0; h < UINT(height); h++)
    { // 가로줄 한 줄씩 복사
        memcpy(&pData[h * ms.RowPitch], &image[h * width * pixelSize], width * pixelSize);
    }
    CONTEXT->Unmap(stagingTexture.Get(), NULL);

    return stagingTexture;
}

bool closeEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon())
{
    return (epsilon > std::abs(a - b));
}

Vec3 DecomposeRotMat(const Matrix& _matRot)
{
    // _mat 을 분해 후 다시 행렬 만들기
    Vec4 vMat[4];

    vMat[0] = Vec4(_matRot._11, _matRot._12, _matRot._13, _matRot._14);
    vMat[1] = Vec4(_matRot._21, _matRot._22, _matRot._23, _matRot._24);
    vMat[2] = Vec4(_matRot._31, _matRot._32, _matRot._33, _matRot._34);
    vMat[3] = Vec4(_matRot._41, _matRot._42, _matRot._43, _matRot._44);

    /*XMStoreFloat4(&vMat[0], _matRot._11);
    XMStoreFloat4(&vMat[1], _matRot.r[1]);
    XMStoreFloat4(&vMat[2], _matRot.r[2]);
    XMStoreFloat4(&vMat[3], _matRot.r[3]);*/

    Vec3 vNewRot;
    if (closeEnough(vMat[0].z, -1.0f))
    {
        float x = 0; // gimbal lock, value of x doesn't matter
        float y = XM_PI / 2;
        float z = x + atan2f(vMat[1].x, vMat[2].x);
        vNewRot = Vec3{x, y, z};
    }
    else if (closeEnough(vMat[0].z, 1.0f))
    {
        float x = 0;
        float y = -XM_PI / 2;
        float z = -x + atan2f(-vMat[1].x, -vMat[2].x);
        vNewRot = Vec3{x, y, z};
    }
    else
    { // two solutions exist
        float y1 = -asinf(vMat[0].z);
        float y2 = XM_PI - y1;

        float x1 = atan2f(vMat[1].z / cosf(y1), vMat[2].z / cosf(y1));
        float x2 = atan2f(vMat[1].z / cosf(y2), vMat[2].z / cosf(y2));

        float z1 = atan2f(vMat[0].y / cosf(y1), vMat[0].x / cosf(y1));
        float z2 = atan2f(vMat[0].y / cosf(y2), vMat[0].x / cosf(y2));

        // choose one solution to return
        // for example the "shortest" rotation
        if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
        {
            vNewRot = Vec3{x1, y1, z1};
        }
        else
        {
            vNewRot = Vec3{x2, y2, z2};
        }
    }
    return vNewRot;
}

void SaveWStringToFile(const wstring& _str, FILE* _File)
{
    UINT iLen = (UINT)_str.length();
    fwrite(&iLen, sizeof(UINT), 1, _File);
    fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWStringFromFile(wstring& _str, FILE* _File)
{
    wchar_t szBuffer[256] = {};

    UINT iLen = 0;
    fread(&iLen, sizeof(UINT), 1, _File);
    fread(szBuffer, sizeof(wchar_t), iLen, _File);

    _str = szBuffer;
}

void SaveAssetRef(Ptr<CAsset> _Asset, FILE* _File)
{
    int i = 0;
    if (nullptr == _Asset)
    {
        fwrite(&i, sizeof(i), 1, _File);
    }
    else
    {
        i = 1;
        fwrite(&i, sizeof(i), 1, _File);
        SaveWStringToFile(_Asset->GetKey(), _File);
        SaveWStringToFile(_Asset->GetRelativePath(), _File);
    }
}

wstring OpenFileDialog(const wstring& strRelativePath, const wchar_t* filter)
{
    wchar_t szName[256] = {};
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CEngine::GetInst()->GetMainWind();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring Path = CPathMgr::GetContentPath();
    Path += strRelativePath;
    ofn.lpstrInitialDir = Path.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn) == TRUE)
        return ofn.lpstrFile;

    return wstring();
}

wstring SaveFileDialog(const wstring& strRelativePath, const wchar_t* filter)
{
    wchar_t szName[256] = {};
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CEngine::GetInst()->GetMainWind();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring Path = CPathMgr::GetContentPath();
    Path += strRelativePath;
    ofn.lpstrInitialDir = Path.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileName(&ofn) == TRUE)
        return ofn.lpstrFile;

    return wstring();
}

void OpenFileDialog(vector<wstring>& _FilesName, const wstring& _RelativePath)
{
    IFileOpenDialog* pFileDialog;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileDialog));
    if (FAILED(hr))
    {
        LOG(Error, "Failed to create FileOpenDialog instance");
        return;
    }

    // 다중 선택 가능 설정
    DWORD dwOptions;
    hr = pFileDialog->GetOptions(&dwOptions);
    if (SUCCEEDED(hr))
    {
        hr = pFileDialog->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
    }

    if (SUCCEEDED(hr))
    {
        // 원하는 초기 디렉토리 경로를 여기에 설정
        wstring Path = CPathMgr::GetContentPath();
        Path += _RelativePath;

        PWSTR initialDir = (PWSTR)(Path.c_str());

        IShellItem* pInitialDirItem = nullptr;

        hr = SHCreateItemFromParsingName(initialDir, NULL, IID_IShellItem, reinterpret_cast<void**>(&pInitialDirItem));

        if (FAILED(hr))
        {
            LOG(Error, "Failed to set initial Directory Path");
            pFileDialog->Release();
            return;
        }

        // 파일 대화 상자 인터페이스에 초기 디렉토리 설정
        hr = pFileDialog->SetFolder(pInitialDirItem);
        pInitialDirItem->Release();
    }

    // 파일 필터 설정
    COMDLG_FILTERSPEC fileTypes[] = {{L"All Files", L"*.*"}, {L"Text Files", L"*.txt"}, {L"FBX Files", L"*.fbx"}};
    hr = pFileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
    if (FAILED(hr))
    {
        LOG(Error, "Failed to set file types");
        pFileDialog->Release();
        return;
    }

    // 대화 상자 열기
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr))
    {
        LOG(Error, "Failed to open FileOpenDialog");
        pFileDialog->Release();
        return;
    }

    // 선택된 파일 목록 가져오기
    IShellItemArray* pItems;
    hr = pFileDialog->GetResults(&pItems);
    if (FAILED(hr))
    {
        LOG(Error, "Failed to get selected items");
        pFileDialog->Release();
        return;
    }

    // 선택된 파일들의 경로 가져오기
    DWORD itemCount;
    hr = pItems->GetCount(&itemCount);
    if (SUCCEEDED(hr))
    {
        for (DWORD i = 0; i < itemCount; ++i)
        {
            IShellItem* pItem;
            hr = pItems->GetItemAt(i, &pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr))
                {
                    _FilesName.push_back(pszFilePath);
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
    }

    pItems->Release();
    pFileDialog->Release();
}

Vec2 LoadMeta(const wstring& _strMetaRelativePath)
{
    Vec2 retVec = {-1, -1};
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _strMetaRelativePath.c_str(), L"r");

    if (nullptr == pFile)
    {
        LOG(Error, "Meta File No Exist!");
        return {};
    }

    // Animation 이름 로드

    while (true)
    {
        wchar_t szRead[256] = {};
        float tmpfloat = -1.f;

        if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
        {
            break;
        }

        if (!wcscmp(szRead, L"m_Offset:"))
        {
            while (true)
            {
                fwscanf_s(pFile, L"%s", szRead, 256);

                if (!wcscmp(szRead, L"{x:"))
                {
                    fwscanf_s(pFile, L"%f", &retVec.x);
                    retVec.x *= -1;
                }
                if (!wcscmp(szRead, L"y:"))
                {
                    fwscanf_s(pFile, L"%s", szRead, 256);

                    int length = (int)wcslen(szRead);

                    // 끝에 한글자 잘라야됨 1.24} 라고 되어있음
                    if (length > 0)
                    {
                        szRead[length - 1] = '\0';
                    }

                    wchar_t* end;
                    float tmp = wcstof(szRead, &end);

                    if (*end == L'\0')
                    {
                        retVec.y = tmp;
                    }

                    return retVec;
                }

                // 탈출 조건
                if (!wcscmp(szRead, L"m_Border:"))
                {
                    break;
                }
            }
        }
        // 탈출 조건
        if (!wcscmp(szRead, L"m_Border:"))
            break;
    }

    fclose(pFile);

    return retVec;
}

void ImGui_DrawVec3Control(const string& label, Vec3& values, float speed, float min, float max, float resetValue, float columnWidth)
{
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 4.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.f);

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, speed, min, max, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

string ImGui_LabelPrefix(const char* const label, float alignment)
{
    float width = ImGui::CalcItemWidth();

    float x = ImGui::GetCursorPosX();
    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::SetCursorPosX(x + width * alignment + ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetNextItemWidth(-1);

    string labelID = "##";
    labelID += label;

    return labelID;
}

//
// Combo with std::vector<string>
//
bool ImGui_ComboUI(const string& caption, string& current_item, const std::vector<string>& items)
{
    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
    {
        for (int n = 0; n < items.size(); n++)
        {
            bool is_selected = (current_item == items[n]);
            if (ImGui::Selectable(items[n].c_str(), is_selected))
            {
                current_item = items[n];
                changed = true;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

bool ImGui_TexturesComboUI(const string& caption, string& current_item)
{
    static ImGuiTextFilter filter;
    const map<wstring, Ptr<CAsset>>& mapTextures = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);

    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
    {
        filter.Draw(ImGui_LabelPrefix("Filter").c_str());
        ImGui::Separator();

        for (const auto& iter : mapTextures)
        {
            string key = ToString(iter.first);
            bool is_selected = (current_item == key);

            if (!filter.PassFilter(key.c_str()))
                continue;

            if (ImGui::Selectable(key.c_str(), is_selected))
            {
                current_item = key;
                changed = true;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }

            // Tooltip
            if (ImGui::IsItemHovered())
            {
                if (ImGui::BeginItemTooltip())
                {
                    Ptr<CTexture> HoveredTex = (CTexture*)iter.second.Get();
                    ImGui::Text(ToString(HoveredTex->GetName()).c_str());
                    ImGui::Text("Texture Width : %d", HoveredTex->GetWidth());
                    ImGui::Text("Texture Height : %d", HoveredTex->GetHeight());
                    ImGui::Image(((CTexture*)HoveredTex.Get())->GetSRV().Get(), ImVec2(100, 100));
                    ImGui::EndTooltip();
                }
            }
        }

        ImGui::EndCombo();
    }

    return changed;
}

bool ImGui_AlignButton(const char* label, float alignment)
{
    alignment = std::clamp(alignment, 0.f, 1.f);

    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}

void ImGui_InputText(const char* label, const string& Text, float alignment)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    if (!Text.empty())
        strcpy_s(buffer, sizeof(buffer), Text.c_str());

    ImGui::InputText(ImGui_LabelPrefix(label, alignment).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
}

void ImGui_SetWindowClass(EDITOR_TYPE _Type)
{
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID(ToString(CEditorMgr::GetInst()->GetEditor(_Type)->GetName()).c_str());
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);
}
