#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTransform.h"

CRenderMgr::CRenderMgr()
    : m_pDebugObj(nullptr)
    , m_Sampling(nullptr)
    , m_BlurX(nullptr)
    , m_BlurY(nullptr)
{
}

CRenderMgr::~CRenderMgr()
{
    if (nullptr != m_pDebugObj)
    {
        delete m_pDebugObj;
        m_pDebugObj = nullptr;
    }

    if (nullptr != m_Sampling)
    {
        delete m_Sampling;
        m_Sampling = nullptr;
    }

    if (nullptr != m_BlurX)
    {
        delete m_BlurX;
        m_BlurX = nullptr;
    }

    if (nullptr != m_BlurY)
    {
        delete m_BlurY;
        m_BlurY = nullptr;
    }

    if (nullptr != m_Combine)
    {
        delete m_Combine;
        m_Combine = nullptr;
    }
}

void CRenderMgr::tick()
{
    render();

    render_debug();

    CopyRenderTarget();
}

void CRenderMgr::render()
{
    m_vecCam[0]->render();

    render_postprocess();

    for (size_t i = 1; i < m_vecCam.size(); ++i)
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
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
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
        m_pDebugObj->Transform()->UpdateData();

        m_pDebugObj->render();
    }
}

void CRenderMgr::render_postprocess()
{
    // Postprocessing 전 원본 저장
    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_OriginRTCopyTex->GetTex2D().Get(), pTex->GetTex2D().Get());

    for (size_t i = 0; i < 1; ++i)
    {
        if (nullptr != m_Sampling)
        {
            CopyRenderTarget();
            m_Sampling->render();
        }
    }

    for (size_t i = 0; i < 5; ++i)
    {
        if (nullptr != m_BlurX)
        {
            CopyRenderTarget();
            m_BlurX->render();
        }
        if (nullptr != m_BlurY)
        {
            CopyRenderTarget();
            m_BlurY->render();
        }
    }

    for (size_t i = 0; i < 1; ++i)
    {
        if (nullptr != m_Combine)
        {
            CopyRenderTarget();
            m_Combine->render();
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
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pTex->GetTex2D().Get());
}

void CRenderMgr::CreateRTCopyTex(Vec2 Resolution)
{
    assert(!(m_RTCopyTex.Get() || m_OriginRTCopyTex.Get()));
    m_RTCopyTex = new CTexture;
    m_RTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                      DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE,
                                                      D3D11_USAGE_DEFAULT);
    m_OriginRTCopyTex = new CTexture;
    m_OriginRTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"OriginRTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                            DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE,
                                                            D3D11_USAGE_DEFAULT);

    CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Sampling")->SetTexParam(TEX_0, m_RTCopyTex);
    CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurX")->SetTexParam(TEX_0, m_RTCopyTex);
    CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurY")->SetTexParam(TEX_0, m_RTCopyTex);

    CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Combine")->SetTexParam(TEX_0, m_RTCopyTex);
    CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Combine")->SetTexParam(TEX_1, m_OriginRTCopyTex);
}

void CRenderMgr::Resize(Vec2 Resolution)
{
    m_RTCopyTex = nullptr;
    m_OriginRTCopyTex = nullptr;
    CreateRTCopyTex(Resolution);
}
