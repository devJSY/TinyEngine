#include "pch.h"
#include "CLevelEditor.h"
#include "CEngine.h"
#include "CDevice.h"

CLevelEditor::CLevelEditor()
    : CEditor(EDITOR_TYPE::LEVEL)
{
}

CLevelEditor::~CLevelEditor()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CLevelEditor::begin()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    //  io.ConfigViewportsNoAutoMerge = true;
    //  io.ConfigViewportsNoTaskBarIcon = true;
    //  io.ConfigViewportsNoDefaultParent = true;
    //  io.ConfigDockingAlwaysTabBar = true;
    //  io.ConfigDockingTransparentPayload = true;
    //  io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T
    //  WORK AS EXPECTED. DON'T USE IN USER APP! io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; //
    //  FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(CEngine::GetInst()->GetMainWind());
    ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());
}

void CLevelEditor::tick()
{
    // ImGUI Render Start
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void CLevelEditor::finaltick()
{
    // ImGUI Rendering
    bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("Test");
    ImGui::Text("Hello World!");
    ImGui::End();
}

void CLevelEditor::render()
{
    // Viewport บนป็
    CDevice::GetInst()->CopyToViewport();
    ID3D11ShaderResourceView* ViewportSRV = CDevice::GetInst()->GetViewportSRV();

    Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
    ImGui::Begin("Viewport");
    ImGui::Image((void*)ViewportSRV, ImVec2(Resolution.x, Resolution.y));
    ImGui::End();

    // Rendering
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
