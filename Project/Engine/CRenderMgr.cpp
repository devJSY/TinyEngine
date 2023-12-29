#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTransform.h"

CRenderMgr::CRenderMgr()
    : m_pDebugObj(nullptr)
{
}

CRenderMgr::~CRenderMgr()
{
    if (nullptr != m_pDebugObj)
    {
        delete m_pDebugObj;
        m_pDebugObj = nullptr;
    }
}

void CRenderMgr::tick()
{
    render();

    render_debug();
}

void CRenderMgr::render()
{
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        m_vecCam[i]->render();
    }
}

void CRenderMgr::render_debug()
{
    g_Transform.matView = m_vecCam[0]->GetViewMat();
    g_Transform.matProj = m_vecCam[0]->GetProjMat();

    list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
    for (; iter != m_DbgShapeInfo.end(); ++iter)
    {
        switch ((*iter).eShape)
        {
        case DEBUG_SHAPE::RECT:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SquareMesh"));
            break;
        case DEBUG_SHAPE::CIRCLE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
            break;
        case DEBUG_SHAPE::CUBE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
            break;
        case DEBUG_SHAPE::SPHERE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
            break;
        default:
            break;
        }

        m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
        m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);

        m_pDebugObj->render();
    }
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
    if (m_vecCam.size() <= _Idx + 1)
    {
        m_vecCam.resize(_Idx + 1);
    }

    // 동일한 우선순위의 카메라가 이미 존재하면 assert
    assert(nullptr == m_vecCam[_Idx]);

    m_vecCam[_Idx] = _Cam;
}

CCamera* CRenderMgr::GetCamera(int _Idx) const
{
    if (m_vecCam.size() <= _Idx)
        return nullptr;

    return m_vecCam[_Idx];
}
