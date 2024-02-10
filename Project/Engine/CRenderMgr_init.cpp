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
    CreateBloomTextures(vRenderResolution);

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

    m_mirror = new CGameObject;
    m_mirror->AddComponent(new CTransform);
    m_mirror->AddComponent(new CMeshRender);

    m_mirror->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_mirror->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BasicMtrl"));
    m_mirror->MeshRender()->GetMaterial()->SetMaterialCoefficient(
        Vec4(0.f, 1.f, 0.f, 0.f), Vec4(0.5f, 0.5f, 0.5f, 1.f), Vec4(0.5f, 0.5f, 0.5f, 1.f), 0.f, 0.f, Vec4());
    m_mirror->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
    m_mirror->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 1.f));

    m_mirrorPlane = SimpleMath::Plane(m_mirror->Transform()->GetWorldPos(), Vector3(0.0f, 0.0f, -1.0f));
}