#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLayer.h"
#include "CRenderMgr.h"
#include "CTaskMgr.h"

#include "CLevelSaveLoad.h"
#include "components.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
    if (nullptr != m_CurLevel)
    {
        delete m_CurLevel;
        m_CurLevel = nullptr;
    }
}

void CLevelMgr::init()
{
    m_CurLevel = new CLevel;
    m_CurLevel->ChangeState(LEVEL_STATE::STOP);
}

void CLevelMgr::tick()
{
    if (nullptr == m_CurLevel)
        return;

    // 이전 프레임에 등록된 오브젝트들 clear
    m_CurLevel->clear();

    // Play, Simulate 상태 이거나
    // Pause 상태에선 Step중인 경우에만 재생
    LEVEL_STATE State = m_CurLevel->GetState();
    if (State == LEVEL_STATE::PLAY || State == LEVEL_STATE::SIMULATE || (State == LEVEL_STATE::PAUSE && m_CurLevel->m_StepFrames > 0))
    {
        m_CurLevel->tick();
    }

    m_CurLevel->finaltick();
}

CLevel* CLevelMgr::CreateDefaultLevel()
{
    CLevel* NewLevel = new CLevel;
    NewLevel->SetName(L"Default Level");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        NewLevel->GetLayer(i)->SetName(Name);
    }

    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CMeshRender);
    pCamObj->AddComponent(new CCamera);

    pCamObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
    pCamObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CameraIconMtrl"), 0);
    pCamObj->MeshRender()->SetFrustumCheck(false);
    pCamObj->MeshRender()->SetCastShadow(false);

    pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(NewLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(false);

    NewLevel->AddObject(pCamObj, 0);

    // UI 만 렌더링
    CGameObject* pUICamObj = new CGameObject;
    pUICamObj->SetName(L"UI Camera");
    pUICamObj->AddComponent(new CTransform);
    pUICamObj->AddComponent(new CMeshRender);
    pUICamObj->AddComponent(new CCamera);

    pUICamObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
    pUICamObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CameraIconMtrl"), 0);
    pUICamObj->MeshRender()->SetFrustumCheck(false);
    pUICamObj->MeshRender()->SetCastShadow(false);

    pUICamObj->Camera()->SetCameraPriority(1);
    pUICamObj->Camera()->LayerMask(NewLevel, L"UI", true);

    NewLevel->AddObject(pUICamObj, 0);

    // SkyBox
    CGameObject* pSkyBoxObj = new CGameObject;
    pSkyBoxObj->SetName(L"SkyBox");
    pSkyBoxObj->AddComponent(new CTransform);
    pSkyBoxObj->AddComponent(new CSkyBox);

    pSkyBoxObj->Transform()->SetLocalPos(Vec3(5000.f, 0.f, 0.f));

    pSkyBoxObj->SkyBox()->SetEnvTex(
        CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\moonless\\moonlessEnvHDR.dds", L"Texture\\skybox\\moonless\\moonlessEnvHDR.dds"));

    pSkyBoxObj->SkyBox()->SetBrdfTex(
        CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\moonless\\moonlessBrdf.dds", L"Texture\\skybox\\moonless\\moonlessBrdf.dds"));

    pSkyBoxObj->SkyBox()->SetDiffuseTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\moonless\\moonlessDiffuseHDR.dds",
                                                                             L"Texture\\skybox\\moonless\\moonlessDiffuseHDR.dds"));

    pSkyBoxObj->SkyBox()->SetSpecularTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\moonless\\moonlessSpecularHDR.dds",
                                                                              L"Texture\\skybox\\moonless\\moonlessSpecularHDR.dds"));

    NewLevel->AddObject(pSkyBoxObj, 15);

    return NewLevel;
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _StartState)
{
    if (m_CurLevel == _NextLevel)
        return;

    if (nullptr != m_CurLevel)
        delete m_CurLevel;

    m_CurLevel = _NextLevel;

    // Physics World 초기화를 위한 트랜스폼 정보 게산
    m_CurLevel->finaltick();
    CRenderMgr::GetInst()->Clear(); // finaltick으로 등록된 광원 삭제

    if (nullptr != m_CurLevel)
        m_CurLevel->ChangeState(_StartState);
}
