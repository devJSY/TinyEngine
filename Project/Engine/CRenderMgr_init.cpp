#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"

void CRenderMgr::init()
{
    m_pDebugObj = new CGameObject;
    m_pDebugObj->AddComponent(new CTransform);
    m_pDebugObj->AddComponent(new CMeshRender);

    ID3D11Texture2D* ptex = CDevice::GetInst()->GetRenderTargetTexture().Get();
    Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

    m_RTCopyTex = new CTexture;

    m_RTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                      DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE,
                                                      D3D11_USAGE_DEFAULT);
}