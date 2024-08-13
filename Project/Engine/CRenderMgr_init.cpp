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

    m_LightBuffer = new CStructuredBuffer;
    m_LightBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    m_pDebugObj = new CGameObject;
    m_pDebugObj->AddComponent(new CTransform);
    m_pDebugObj->AddComponent(new CMeshRender);
    m_pDebugObj->MeshRender()->SetFrustumCheck(false);
    m_pDebugObj->MeshRender()->SetCastShadow(false);

    Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
    CreateRTCopyTex(vRenderResolution);
    CreateIDMapTex(vRenderResolution);
    CreateDepthOnlyTex(vRenderResolution);
    CreateMotionVectorTex(vRenderResolution);
    CreatePostProcessTex(vRenderResolution);
    CreateCameraPreviewTex(vRenderResolution);

    m_FloatRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatRenderTargetTexture");
    CreateMRT(vRenderResolution);

    // Noise Texture Load
    m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_01.jpg", L"Texture\\noise\\noise_01.jpg"));
    m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_02.jpg", L"Texture\\noise\\noise_02.jpg"));
    m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg"));

    m_vecNoiseTex[1]->UpdateData(16);
    m_vecNoiseTex[1]->UpdateData_CS_SRV(16);
    g_Global.g_NoiseTexResolution = Vec2(m_vecNoiseTex[1]->GetWidth(), m_vecNoiseTex[1]->GetHeight());

    // Post Processing
    CreateBloomTextures(vRenderResolution);

    // LDRI Bloom
    m_SamplingObj = new CGameObject;
    m_SamplingObj->AddComponent(new CTransform);
    m_SamplingObj->AddComponent(new CMeshRender);
    m_SamplingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_SamplingObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SamplingMtrl"), 0);

    m_BlurXObj = new CGameObject;
    m_BlurXObj->AddComponent(new CTransform);
    m_BlurXObj->AddComponent(new CMeshRender);
    m_BlurXObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_BlurXObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurXMtrl"), 0);

    m_BlurYObj = new CGameObject;
    m_BlurYObj->AddComponent(new CTransform);
    m_BlurYObj->AddComponent(new CMeshRender);
    m_BlurYObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_BlurYObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurYMtrl"), 0);

    m_CombineObj = new CGameObject;
    m_CombineObj->AddComponent(new CTransform);
    m_CombineObj->AddComponent(new CMeshRender);
    m_CombineObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_CombineObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CombineMtrl"), 0);
    m_CombineObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_RTCopyTex);
    m_CombineObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, m_BloomRTTex_LDRI);

    // HDRI Bloom
    m_BloomDownObj = new CGameObject;
    m_BloomDownObj->AddComponent(new CTransform);
    m_BloomDownObj->AddComponent(new CMeshRender);
    m_BloomDownObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_BloomDownObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomDownMtrl"), 0);

    m_BloomUpObj = new CGameObject;
    m_BloomUpObj->AddComponent(new CTransform);
    m_BloomUpObj->AddComponent(new CMeshRender);
    m_BloomUpObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_BloomUpObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomUpMtrl"), 0);

    m_ToneMappingObj = new CGameObject;
    m_ToneMappingObj->AddComponent(new CTransform);
    m_ToneMappingObj->AddComponent(new CMeshRender);
    m_ToneMappingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_ToneMappingObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ToneMappingMtrl"), 0);
    m_ToneMappingObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_FloatRTTex);
    m_ToneMappingObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, m_PostProcessTex_HDRI);

    // PostEffect
    m_PostEffectObj = new CGameObject;
    m_PostEffectObj->AddComponent(new CTransform);
    m_PostEffectObj->AddComponent(new CMeshRender);
    m_PostEffectObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_PostEffectObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PostEffectMtrl"), 0);
    m_PostEffectObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_FloatRTTex);

    // DepthMasking
    m_DepthMaskingObj = new CGameObject;
    m_DepthMaskingObj->AddComponent(new CTransform);
    m_DepthMaskingObj->AddComponent(new CMeshRender);
    m_DepthMaskingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_DepthMaskingObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DepthMaskingMtrl"), 0);
    m_DepthMaskingObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_FloatRTTex);
    m_DepthMaskingObj->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.25f);

    // Depth of Field
    m_DOFObj = new CGameObject;
    m_DOFObj->AddComponent(new CTransform);
    m_DOFObj->AddComponent(new CMeshRender);
    m_DOFObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_DOFObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DOFMtrl"), 0);
    m_DOFObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_RTCopyTex);

    // Motion Blur
    m_MotionBlurObj = new CGameObject;
    m_MotionBlurObj->AddComponent(new CTransform);
    m_MotionBlurObj->AddComponent(new CMeshRender);
    m_MotionBlurObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_MotionBlurObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"MotionBlurMtrl"), 0);
    m_MotionBlurObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_MotionVectorTex);
}