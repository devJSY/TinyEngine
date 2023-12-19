#include "pch.h"
#include "CDevice.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"

#include "CLevel.h"

#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

CLevelMgr::CLevelMgr()
{
}

CLevelMgr::~CLevelMgr()
{
    if (nullptr != m_CurLevel)
        delete m_CurLevel;
}

void CLevelMgr::init()
{
    m_CurLevel = new CLevel;

    m_CurLevel->begin();

    // Camera Object ����
    CGameObject* pCamObj = new CGameObject;
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(new CCameraMoveScript);

    pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
    pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

    m_CurLevel->AddObject(pCamObj, 0);

    // GameObject ����
    CGameObject* pObj = nullptr;

    pObj = new CGameObject;
    pObj->SetName(L"Player");

    pObj->AddComponent(new CTransform);
    pObj->AddComponent(new CMeshRender);
    pObj->AddComponent(new CPlayerScript);

    pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.5f));
    pObj->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));

    pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

    m_CurLevel->AddObject(pObj, 0);
}

void CLevelMgr::tick()
{
    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->tick();
    m_CurLevel->finaltick();
}

void CLevelMgr::render()
{
    if (nullptr == m_CurLevel)
        return;

    float ClearColor[4] = {0.3f, 0.3f, 0.3f, 1.f};
    CDevice::GetInst()->ClearRenderTarget(ClearColor);

    m_CurLevel->render();

    CDevice::GetInst()->Present();
}
