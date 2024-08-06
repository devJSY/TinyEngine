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
        if (L"Masking Camera" == (*iter)->GetOwner()->GetName())
        {
            m_mainCam = (*iter);
            break;
        }
    }

    if (m_mainCam == originMainCam)
        return;

    render_Clear(Vec4(0.f, 0.f, 0.f, 1.f));

    UpdateData();

    m_mainCam->SortObject();
    m_mainCam->render_Forward();

    Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_MaskingTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
    m_mainCam = originMainCam;
}