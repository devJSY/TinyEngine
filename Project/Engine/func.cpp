#include "pch.h"
#include "ShObjIdl_core.h" // FileDialog

#include "CDevice.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
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

void GamePlayStatic::DetachObject(CGameObject* _Target)
{
    tTask task = {};
    task.Type = TASK_TYPE::DETACH_OBJECT;
    task.Param_1 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject)
{
    tTask task = {};
    task.Type = TASK_TYPE::ADD_CHILD;
    task.Param_1 = (DWORD_PTR)_ParentObject;
    task.Param_2 = (DWORD_PTR)_ChildObject;
    task.Param_3 = (DWORD_PTR) nullptr;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject, tBoneSocket* _BoneSocket)
{
    tTask task = {};
    task.Type = TASK_TYPE::ADD_CHILD;
    task.Param_1 = (DWORD_PTR)_ParentObject;
    task.Param_2 = (DWORD_PTR)_ChildObject;
    task.Param_3 = (DWORD_PTR)_BoneSocket;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject, const std::wstring& _BoneSocketName)
{
    tBoneSocket* pBoneSocket = nullptr;

    if (nullptr != _ParentObject && nullptr != _ParentObject->Animator() && _ParentObject->Animator()->IsValid())
    {
        pBoneSocket = _ParentObject->Animator()->GetSkeletalMesh()->GetBoneSocket(_BoneSocketName);
    }

    tTask task = {};
    task.Type = TASK_TYPE::ADD_CHILD;
    task.Param_1 = (DWORD_PTR)_ParentObject;
    task.Param_2 = (DWORD_PTR)_ChildObject;
    task.Param_3 = (DWORD_PTR)pBoneSocket;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
    if (nullptr == _Target || _Target->IsDead())
        return;

    tTask task = {};
    task.Type = TASK_TYPE::DELETE_OBJECT;
    task.Param_1 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::WindowResize(int width, int height)
{
    tTask task;
    task.Type = TASK_TYPE::WINDOW_RESIZE;
    task.Param_1 = (DWORD_PTR)width;
    task.Param_2 = (DWORD_PTR)height;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::CloneGameObject(CGameObject* _OriginObj, CGameObject* _OriginParentObj)
{
    tTask task;
    task.Type = TASK_TYPE::CLONE_OBJECT;
    task.Param_1 = (DWORD_PTR)_OriginObj;
    task.Param_2 = (DWORD_PTR)_OriginParentObj;

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
        v.vUV0 = texcoords[i];
        v.vColor = colors[i];
        v.vColor.w = 1.f;

        meshData.vertices.push_back(v);
    }

    meshData.indices.push_back(0);
    meshData.indices.push_back(1);

    Ptr<CMesh> pMesh = new CMesh(true);
    pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugLine(Vec3 _vWorldPos, Vec3 _vDir, float _fLength, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    if (_vDir == Vec3() || _fLength == 0.f)
        return;

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
        v.vUV0 = texcoords[i];
        v.vColor = colors[i];
        v.vColor.w = 1.f;

        meshData.vertices.push_back(v);
    }

    meshData.indices.push_back(0);
    meshData.indices.push_back(1);

    Ptr<CMesh> pMesh = new CMesh(true);
    pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());

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

    Matrix RotationMatrix = Matrix();
    switch (_Axis)
    {
    case AXIS_TYPE::X: {
        RotationMatrix = XMMatrixRotationZ(XM_PIDIV2);
    }
    break;
    case AXIS_TYPE::Y: {
    }
    break;
    case AXIS_TYPE::Z: {
        RotationMatrix = XMMatrixRotationX(XM_PIDIV2);
    }
    break;
    }

    auto meshData = CAssetMgr::GetInst()->MakeWireCapsule(_fRadius, _HalfHeight, 15);
    for (auto& vtx : meshData.vertices)
        vtx.vPos = Vector3::Transform(vtx.vPos, RotationMatrix);

    Ptr<CMesh> pMesh = new CMesh(true);
    pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());

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
        v.vUV0 = texcoords[i];
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

    Ptr<CMesh> pMesh = new CMesh(true);
    pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());

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
        v.vUV0 = texcoords[i];
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

    Ptr<CMesh> pMesh = new CMesh(true);
    pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());

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

    Ptr<CMesh> pMesh = new CMesh(true);
    pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(), (UINT)meshData.indices.size());

    info.pMesh = pMesh;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugMesh(const Matrix& _WorldMat, Ptr<CMesh> _pMesh, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::FRUSTUM;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;
    info.pMesh = _pMesh;

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

void GamePlayStatic::MouseCollisionPicking(Vec2 MousePos)
{
    tTask task = {};
    task.Type = TASK_TYPE::MOUSE_COLLISION_PICKING;
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

void GamePlayStatic::ChangeLevelAsync(const wstring& _LevelPath, LEVEL_STATE _NextState)
{
    CLevelMgr::GetInst()->ChangeLevelAsync(_LevelPath, _NextState);
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
        pSound->Play2D(_Loop, _Volume, _Overlap);
    }
}

void GamePlayStatic::Play3DSound(const wstring& _SoundPath, Vec3 _WorldPos, int _Loop, float _Volume, bool _Overlap, float _Mindistance,
                                 float _Maxdistance)
{
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    if (nullptr != pSound)
    {
        pSound->Play3D(_WorldPos, _Loop, _Volume, _Overlap, _Mindistance, _Maxdistance);
    }
}

void GamePlayStatic::PlayBGM(const wstring& _SoundPath, float _Volume)
{
    static Ptr<CSound> CurBGM = nullptr;

    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    if (nullptr != pSound)
    {
        // 현재 재생 중인 BGM 이라면 볼륨만 변경
        if (CurBGM == pSound)
        {
            CurBGM->SetVolume(_Volume);
        }
        // BGM 변경
        else
        {
            if (nullptr != CurBGM)
            {
                CurBGM->Stop();
            }

            pSound->Play2D(0, _Volume);
            CurBGM = pSound;
        }
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

Vec4 PositionToNDC(Vec3 _Position)
{
    CCamera* _pCam = CRenderMgr::GetInst()->GetMainCamera();
    if (nullptr == _pCam)
    {
        return Vec4(-100, -100, -100, -100);
    }

    Matrix VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();
    Vec4 NDCPos = Vector4::Transform(Vec4(_Position.x, _Position.y, _Position.z, 1.f), VPMatrix);

    // Perspective Division
    NDCPos.x /= NDCPos.w;
    NDCPos.y /= NDCPos.w;
    NDCPos.z /= NDCPos.w;

    return NDCPos;
}

Vec2 NDCToUV(Vec3 _NDC)
{
    Vec2 UV = Vec2(_NDC.x, -_NDC.y); // 텍스춰 좌표와 NDC는 y가 반대
    UV += 1.f;
    UV *= 0.5f;
    return UV;
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

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat)
{
    Matrix mat;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            mat.m[i][j] = (float)_mat.Get(i, j);
        }
    }
    return mat;
}

FbxAMatrix GetFbxMatrixFromMatrix(Matrix& _mat)
{
    FbxAMatrix mat;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            mat[i][j] = _mat.m[i][j];
        }
    }
    return mat;
}

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
    int iRetByte = 0;
    switch (_eFormat)
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        iRetByte = 128;
        break;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        iRetByte = 96;
        break;
    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        iRetByte = 64;
        break;
    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
        iRetByte = 32;
        break;
    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
        iRetByte = 16;
        break;
    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
        iRetByte = 8;
        break;
        // Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
        iRetByte = 128;
        break;
        // Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
    case DXGI_FORMAT_R1_UNORM:
    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        iRetByte = 64;
        break;
        // Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        iRetByte = 32;
        break;
        // These are compressed, but bit-size information is unclear.
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
        iRetByte = 32;
        break;
    case DXGI_FORMAT_UNKNOWN:
    default:
        return -1;
    }

    return iRetByte / 8;
}

bool closeEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon())
{
    return (epsilon > std::abs(a - b));
}

UINT SaveWStringToFile(const wstring& _str, FILE* _File)
{
    UINT MemoryByte = 0;

    UINT iLen = (UINT)_str.length();
    fwrite(&iLen, sizeof(UINT), 1, _File);
    fwrite(_str.c_str(), sizeof(wchar_t), iLen, _File);

    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(wchar_t) * iLen;

    return MemoryByte;
}

UINT LoadWStringFromFile(wstring& _str, FILE* _File)
{
    UINT MemoryByte = 0;

    wchar_t szBuffer[256] = {};

    UINT iLen = 0;
    fread(&iLen, sizeof(UINT), 1, _File);
    MemoryByte += sizeof(UINT);

    // SaveLoad 예외처리 32767자를 넘어가는 문자열은 Read하지 않는다.
    if (32767 > iLen)
    {
        fread(szBuffer, sizeof(wchar_t), iLen, _File);
        _str = szBuffer;
        MemoryByte += sizeof(wchar_t) * iLen;
    }

    return MemoryByte;
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

void OpenFileDialog(vector<wstring>& _FilesName, const wstring& _RelativePath, const vector<std::pair<wstring, wstring>>& filter)
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
    // 파일 필터 설정
    vector<COMDLG_FILTERSPEC> fileTypes;
    for (const auto& f : filter)
    {
        fileTypes.push_back({f.first.c_str(), f.second.c_str()});
    }
    hr = pFileDialog->SetFileTypes((UINT)fileTypes.size(), fileTypes.data());
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
        // 취소, 닫기 버튼 클릭 이회의 경우 로그
        if (hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
        {
            LOG(Error, "Failed to open FileOpenDialog");
        }
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

bool ImGui_DrawVec3Control(const string& label, Vec3& values, float speed, float min, float max, float resetValue, float columnWidth)
{
    bool bDirtyflag = false;

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
    {
        values.x = resetValue;
        bDirtyflag = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f"))
    {
        bDirtyflag = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
    {
        values.y = resetValue;
        bDirtyflag = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f"))
    {
        bDirtyflag = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
    {
        values.z = resetValue;
        bDirtyflag = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Z", &values.z, speed, min, max, "%.2f"))
    {
        bDirtyflag = true;
    }
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    return bDirtyflag;
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

        if (ImGui::Selectable("...", false))
        {
            current_item = string();
            changed = true;
        }

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

bool ImGui_BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = size_arg;
    size.x -= style.FramePadding.x * 2;

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // Render
    const float circleStart = size.x * 0.7f;
    const float circleEnd = size.x;
    const float circleWidth = circleEnd - circleStart;

    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

    const float t = (float)g.Time;
    const float r = size.y / 2;
    const float speed = 1.5f;

    const float a = speed * 0;
    const float b = speed * 0.333f;
    const float c = speed * 0.666f;

    const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
    const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
    const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

    return true;
}

bool ImGui_Spinner(const char* label, float radius, int thickness, const ImU32& color)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start = (int)abs(ImSin((float)g.Time * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + (float)g.Time * 8) * radius, centre.y + ImSin(a + (float)g.Time * 8) * radius));
    }

    window->DrawList->PathStroke(color, false, (float)thickness);

    return true;
}

void ImGui_SetWindowClass(EDITOR_TYPE _Type)
{
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID(ToString(CEditorMgr::GetInst()->GetEditor(_Type)->GetName()).c_str());
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);
}
