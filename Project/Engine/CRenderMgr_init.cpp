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

    CGameObject* pPostProcess = new CGameObject;
    pPostProcess->AddComponent(new CTransform);
    pPostProcess->AddComponent(new CMeshRender);
    pPostProcess->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pPostProcess->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Postprocess"));
    m_vecPost.push_back(pPostProcess);
}