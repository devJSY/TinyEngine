#include "pch.h"
#include "CRenderMgr.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CMeshRender.h"
#include "CTransform.h"

CRenderMgr::CRenderMgr()
    : m_pDebugObj(nullptr)
    , m_bShowDebugRender(false)
    , m_bShowCollider(true)
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
    for (; iter != m_DbgShapeInfo.end();)
    {
        switch ((*iter).eShape)
        {
        case DEBUG_SHAPE::RECT:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
            break;
        case DEBUG_SHAPE::CIRCLE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
            break;
        case DEBUG_SHAPE::CROSS:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CrosshairMesh"));
            break;
        case DEBUG_SHAPE::CUBE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
            break;
        case DEBUG_SHAPE::SPHERE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh_Debug"));
            break;
        default:
            break;
        }

        m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
        m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

        D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
        if (DEBUG_SHAPE::CROSS == (*iter).eShape)
        {
            m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }

        m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
        m_pDebugObj->Transform()->UpdateData();

        m_pDebugObj->render();

        m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);

        // Duration Check
        (*iter).fLifeTime += DT;
        if ((*iter).fDuration <= (*iter).fLifeTime)
        {
            iter = m_DbgShapeInfo.erase(iter);
        }
        else
        {
            ++iter;
        }
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

void CRenderMgr::CopyRenderTarget()
{
    // Viewport 복사
    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    Ptr<CTexture> pRTCopyTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RTCopyTex");
    CONTEXT->CopyResource(pRTCopyTex->GetTex2D().Get(), pTex->GetTex2D().Get());
}

void CRenderMgr::CreateRTCopyTex(Vec2 Resolution)
{
    CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                        DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    CAssetMgr::GetInst()->CreateTexture(L"IDMapTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                        D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT);

    CAssetMgr::GetInst()->CreateTexture(L"IDMapDSTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                        DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::Resize(Vec2 Resolution)
{
    CreateRTCopyTex(Resolution);
}
