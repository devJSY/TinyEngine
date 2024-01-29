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

    // Noise Texture Load
    m_vecNoiseTex.push_back(
        CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\noise\\noise_01.png", L"Textures\\noise\\noise_01.png"));
    m_vecNoiseTex.push_back(
        CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\noise\\noise_02.png", L"Textures\\noise\\noise_02.png"));
    m_vecNoiseTex.push_back(
        CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\noise\\noise_03.jpg", L"Textures\\noise\\noise_03.jpg"));

    m_vecNoiseTex[0]->UpdateData(15);
    m_vecNoiseTex[0]->UpdateData_CS_SRV(15);
}