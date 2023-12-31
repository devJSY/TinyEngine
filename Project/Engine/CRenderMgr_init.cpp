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

    CreateRTCopyTex(CDevice::GetInst()->GetRenderResolution());

    m_Sampling = new CGameObject;
    m_Sampling->AddComponent(new CTransform);
    m_Sampling->AddComponent(new CMeshRender);
    m_Sampling->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_Sampling->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Sampling"));

    m_BlurX = new CGameObject;
    m_BlurX->AddComponent(new CTransform);
    m_BlurX->AddComponent(new CMeshRender);
    m_BlurX->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_BlurX->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurX"));

    m_BlurY = new CGameObject;
    m_BlurY->AddComponent(new CTransform);
    m_BlurY->AddComponent(new CMeshRender);
    m_BlurY->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_BlurY->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurY"));
}