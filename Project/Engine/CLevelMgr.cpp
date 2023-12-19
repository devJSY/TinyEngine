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

    // Camera Object 持失
    CGameObject* pCamObj = new CGameObject;
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(new CCameraMoveScript);

    pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
    pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

    m_CurLevel->AddObject(pCamObj, 0);

    // GameObject 持失
    CGameObject* pObj = nullptr;

    pObj = new CGameObject;
    pObj->SetName(L"Player");

    pObj->AddComponent(new CTransform);
    pObj->AddComponent(new CMeshRender);
    pObj->AddComponent(new CPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

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

    // ImGUI Render
    bool UseImGui = CEngine::GetInst()->IsUseImGui();
    if (UseImGui)
    {
        ImGUIRender();
    }

    float ClearColor[4] = {0.3f, 0.3f, 0.3f, 1.f};
    CDevice::GetInst()->ClearRenderTarget(ClearColor);
    m_CurLevel->render();

    if (UseImGui)
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        //ImGuiIO& io = ImGui::GetIO();
        //// Update and Render additional Platform Windows
        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //{
        //    ImGui::UpdatePlatformWindows();
        //    ImGui::RenderPlatformWindowsDefault();
        //}
    }

    CDevice::GetInst()->Present();
}

void CLevelMgr::ImGUIRender()
{
    bool show_demo_window = true;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to
    // learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // Rendering
    ImGui::Render();
}
