#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"

void CRenderMgr::Create_MaskingTexture(Vec2 Resolution)
{
    m_MaskingTex = CAssetMgr::GetInst()->CreateTexture(L"MaskingTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                       D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::render_masking()
{
    vector<CCamera*>::iterator iter = m_vecCam.begin();

    CCamera* originMainCam = m_mainCam;
 
    for (; iter != m_vecCam.end(); iter++)
    {
        m_mainCam = L"Masking Camera" == (*iter)->GetOwner()->GetName() ? (*iter) : m_mainCam; 
    }

    if (m_mainCam == originMainCam)
        return;

    render_Clear(Vec4(0.f, 0.f, 0.f, 1.f));

    UpdateData();

    // Depth Only Pass
    m_mainCam->SortShadowMapObject(MOBILITY_TYPE::STATIC | MOBILITY_TYPE::MOVABLE);
    m_mainCam->render_DepthOnly(m_DepthOnlyTex);

    // Dynamic Shadow Depth Map
    render_DynamicShadowDepth();

    m_mainCam->SortObject();
    if (0 == m_mainCam->GetCameraPriority())
        m_mainCam->render_Deferred();
    m_mainCam->render_Forward();

    Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_MaskingTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
    m_mainCam = originMainCam;

    // Light DepthMap Clear
    CTexture::Clear(23);
}