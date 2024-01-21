#include "pch.h"
#include "CSpriteEditor.h"
#include "CAssetMgr.h"

#include "CKeyMgr.h"

CSpriteEditor::CSpriteEditor()
    : CEditor(EDITOR_TYPE::SPRITE)
    , m_pTex()
    , m_ViewportScale(1.f)
{
}

CSpriteEditor::~CSpriteEditor()
{
}

void CSpriteEditor::init()
{
}

void CSpriteEditor::render()
{
    ImGuiSetWindowClass_SpriteEditor();
    DrawViewprot();

    ImGuiSetWindowClass_SpriteEditor();
    DrawDetails();

    ImGuiSetWindowClass_SpriteEditor();
    DrawSpriteList();
}

void CSpriteEditor::DrawViewprot()
{
    ImGui::Begin("Viewprot##SpriteEditor");

    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;
    static bool opt_enable_context_menu = true;
    static bool adding_line = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
    ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use
    // IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();    // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to what's available
    if (canvas_sz.x < 50.0f)
        canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f)
        canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered();                            // Hovered
    const bool is_active = ImGui::IsItemActive();                              // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Add first and second point
    if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImRect rect;

        rect.Min = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) / m_ViewportScale;
        rect.Max = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) / m_ViewportScale;

        m_Sprites.push_back(rect);
        adding_line = true;
    }
    if (adding_line)
    {
        m_Sprites.back().Max = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) / m_ViewportScale;
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
            adding_line = false;
    }

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
    const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    if (ImGui::BeginPopup("context"))
    {
        if (adding_line)
            m_Sprites.resize(m_Sprites.size() - 1);
        adding_line = false;
        if (ImGui::MenuItem("Remove one", NULL, false, m_Sprites.Size > 0))
        {
            m_Sprites.resize(m_Sprites.size() - 1);
        }
        if (ImGui::MenuItem("Remove all", NULL, false, m_Sprites.Size > 0))
        {
            m_Sprites.clear();
        }
        ImGui::EndPopup();
    }

    // Scale
    if (ImGui::IsWindowHovered())
    {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel > 0)
            m_ViewportScale *= 1.1f;
        else if (wheel < 0)
            m_ViewportScale *= 0.9f;
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = 32.0f;

        for (float x = fmodf(scrolling.x, GRID_STEP * m_ViewportScale); x < canvas_sz.x;
             x += GRID_STEP * m_ViewportScale)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP * m_ViewportScale); y < canvas_sz.y;
             y += GRID_STEP * m_ViewportScale)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
    }

    // Texture Render
    if (nullptr != m_pTex.Get())
        draw_list->AddImage((void*)m_pTex->GetSRV().Get(), origin,
                            origin + ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight()) * m_ViewportScale);

    for (int n = 0; n < m_Sprites.Size; n++)
    {
        // 각 점에 m_ViewprotScale을 곱하여 위치를 이동시킴
        ImVec2 min =
            ImVec2(origin.x + m_Sprites[n].Min.x * m_ViewportScale, origin.y + m_Sprites[n].Min.y * m_ViewportScale);
        ImVec2 max =
            ImVec2(origin.x + m_Sprites[n].Max.x * m_ViewportScale, origin.y + m_Sprites[n].Max.y * m_ViewportScale);

        draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
    }

    draw_list->PopClipRect();

    ImGui::End();
}

void CSpriteEditor::DrawDetails()
{
    ImGui::Begin("Details##SpriteEditor");

    const map<wstring, Ptr<CAsset>>& mapTextures = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);
    vector<string> names;
    for (const auto& iter : mapTextures)
    {
        names.push_back(ToString(iter.first));
    }

    std::string CurTextureName;

    if (nullptr != m_pTex.Get())
        CurTextureName = ToString(m_pTex->GetKey());

    if (ImGuiComboUI(ImGuiLabelPrefix("Source Texture").c_str(), CurTextureName, names))
    {
        m_pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(CurTextureName));
    }

    if (nullptr != m_pTex.Get())
        ImGui::Image((void*)m_pTex->GetSRV().Get(), ImVec2(250.f, 250.f));

    ImGui::End();
}

void CSpriteEditor::DrawSpriteList()
{
    if (nullptr == m_pTex.Get())
        return;

    ImGui::Begin("Sprite List##SpriteEditor");

    for (int i = 1; i <= m_Sprites.Size; i++)
    {
        int idx = i - 1;
        ImVec2 TextureSize = ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

        ImGui::Image((void*)m_pTex->GetSRV().Get(), ImVec2(100.f, 100.f), m_Sprites[idx].Min / TextureSize,
                     m_Sprites[idx].Max / TextureSize, ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f));

        if (i % 5 != 0)
            ImGui::SameLine();
    }

    ImGui::End();
}

void CSpriteEditor::render(bool* open)
{
    // =====================================
    // DockSpace
    // =====================================
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetMainViewport()->ID;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther;
    window_class.DockingAllowUnclassed = true;
    ImGui::SetNextWindowClass(&window_class);

    if (!ImGui::Begin(ToString(GetName()).c_str(), open))
    {
        ImGui::End();
        return;
    }

    ImGuiID dockSpace = ImGui::GetID("Sprite Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Sprite Editor Render
    // =====================================
    render();

    ImGui::End();
}
