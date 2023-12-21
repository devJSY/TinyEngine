#include "pch.h"
#include "CDevice.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"

#include "CLevel.h"

#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CEngine.h"
#include "CTaskMgr.h"

#include "CTexture.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
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

    // Camera Object 积己
    CGameObject* pCamObj = new CGameObject;
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(new CCameraMoveScript);

    pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
    pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

    m_CurLevel->AddObject(pCamObj, 0);

    // GameObject 积己
    CGameObject* pObj = nullptr;

    pObj = new CGameObject;
    pObj->SetName(L"Player");

    pObj->AddComponent(new CTransform);
    pObj->AddComponent(new CMeshRender);
    pObj->AddComponent(new CPlayerScript);

    pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

    pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

    m_CurLevel->AddObject(pObj, 0);
}

void CLevelMgr::tick()
{
    // 扩档快 荤捞令 眉农
    RECT rect;
    if (GetClientRect(CEngine::GetInst()->GetMainWind(), &rect))
    {
        Vec2 Resolution = CEngine::GetInst()->GetResolution();
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        if (Resolution.x != width || Resolution.y != height)
        {
            FTask task;
            task.Type = TASK_TYPE::WINDOW_RESIZE;
            task.Param_1 = (INT_PTR)width;
            task.Param_2 = (INT_PTR)height;

            CTaskMgr::GetInst()->AddTask(task);
        }
    }

    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->tick();
    m_CurLevel->finaltick();
}

void CLevelMgr::render()
{
    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->render();
}
