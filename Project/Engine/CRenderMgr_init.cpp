#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"

void CRenderMgr::init()
{
    m_Light2DBuffer = new CStructuredBuffer;
    m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    m_Light3DBuffer = new CStructuredBuffer;
    m_Light3DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    m_pDebugObj = new CGameObject;
    m_pDebugObj->AddComponent(new CTransform);
    m_pDebugObj->AddComponent(new CMeshRender);

    Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
    CreateRTCopyTex(vRenderResolution);
    CreateIDMapTex(vRenderResolution);
    CreatePostProcessTex(vRenderResolution);
    m_FloatRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatRenderTargetTexture");

    // Noise Texture Load
    m_vecNoiseTex.push_back(
        CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_01.jpg", L"Texture\\noise\\noise_01.jpg"));
    m_vecNoiseTex.push_back(
        CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_02.jpg", L"Texture\\noise\\noise_02.jpg"));
    m_vecNoiseTex.push_back(
        CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg"));

    m_vecNoiseTex[1]->UpdateData(15);
    m_vecNoiseTex[1]->UpdateData_CS_SRV(15);
    g_Global.g_NoiseTexResolution = Vec2(m_vecNoiseTex[1]->GetWidth(), m_vecNoiseTex[1]->GetHeight());

    // Post Processing
    for (int i = 0; i < bloomLevels - 1; i++)
    {
        int div = int(pow(2, 1 + i));
        m_BloomTextures.push_back(CAssetMgr::GetInst()->CreateTexture(
            L"BloomTexture " + std::to_wstring(i), UINT(vRenderResolution.x / div), UINT(vRenderResolution.y / div),
            DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
            D3D11_USAGE_DEFAULT));
    }

    for (int i = 0; i < bloomLevels - 1; i++)
    {
        CGameObject* bloomDownObj = new CGameObject;
        bloomDownObj->AddComponent(new CTransform);
        bloomDownObj->AddComponent(new CMeshRender);

        bloomDownObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        bloomDownObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomDownMtrl"));

        m_BloomDownFilters.push_back(bloomDownObj);
    }

    for (int i = 0; i < bloomLevels - 1; i++)
    {
        CGameObject* bloomUpObj = new CGameObject;
        bloomUpObj->AddComponent(new CTransform);
        bloomUpObj->AddComponent(new CMeshRender);

        bloomUpObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        bloomUpObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomUpMtrl"));

        m_BloomUpFilters.push_back(bloomUpObj);
    }

    m_ToneMappingObj = new CGameObject;
    m_ToneMappingObj->AddComponent(new CTransform);
    m_ToneMappingObj->AddComponent(new CMeshRender);

    m_ToneMappingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_ToneMappingObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ToneMappingMtrl"));
    m_ToneMappingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_FloatRTTex);
    m_ToneMappingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, m_PostProcessTex);
}