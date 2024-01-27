#include "pch.h"
#include "CTileMapEditor.h"

CTileMapEditor::CTileMapEditor()
    : CEditor(EDITOR_TYPE::TILEMAP)
    , m_TileMapObj(nullptr)
    , m_ViewportOffset()
    , m_ViewportScale(1.f)
{
}

CTileMapEditor::~CTileMapEditor()
{
}

void CTileMapEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("TileMap Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // TileMap Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CTileMapEditor::render()
{
    ImGuiSetWindowClass_TileMapEditor();
    DrawViewport();

    ImGuiSetWindowClass_TileMapEditor();
    DrawDetails();

    ImGuiSetWindowClass_TileMapEditor();
    DrawTileSet();
}

void CTileMapEditor::DrawViewport()
{
    ImGui::Begin("Viewport##TileMapEditor");

    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
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
    ImGui::InvisibleButton("canvas", canvas_sz,
                           ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight); // Held
    const ImVec2 origin(canvas_p0.x + m_ViewportOffset.x, canvas_p0.y + m_ViewportOffset.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    {
        m_ViewportOffset.x += io.MouseDelta.x;
        m_ViewportOffset.y += io.MouseDelta.y;
    }

    // =================================
    // Rendering
    // =================================

    if (nullptr != m_TileMapObj && nullptr != m_TileMapObj->TileMap())
    {
        ImVec2 RenderSize =
            ImVec2(m_TileMapObj->TileMap()->m_vTileRenderSize.x, m_TileMapObj->TileMap()->m_vTileRenderSize.y);

        for (int y = 0; y < (int)m_TileMapObj->TileMap()->m_iTileCountY; y++)
        {
            for (int x = 0; x < (int)m_TileMapObj->TileMap()->m_iTileCountX; x++)
            {
                ImVec2 LT;
                LT.x = x * RenderSize.x * m_ViewportScale;
                LT.y = y * RenderSize.y * m_ViewportScale;

                ImVec2 RB;
                RB.x = LT.x + RenderSize.x * m_ViewportScale;
                RB.y = LT.y + RenderSize.y * m_ViewportScale;

                draw_list->AddRect(origin + LT, origin + RB, IM_COL32(0, 255, 0, 255));

                /*               if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                               {
                                   m_TileMapObj->TileMap()->SetTileIndex(y, x, 0);
                               }*/
            }
        }

        draw_list->AddRect(mouse_pos_in_canvas, mouse_pos_in_canvas + (RenderSize * m_ViewportScale),
                           IM_COL32(255, 0, 0, 255));
    }

    // =================================
    // Scale Set
    // =================================
    if (ImGui::IsWindowHovered())
    {
        float wheel = ImGui::GetIO().MouseWheel;

        // 마우스의 현재 위치를 캔버스 기준으로 계산
        ImVec2 mouse_pos_in_canvas_relative =
            ImVec2((io.MousePos.x - canvas_p0.x - m_ViewportOffset.x) / m_ViewportScale,
                   (io.MousePos.y - canvas_p0.y - m_ViewportOffset.y) / m_ViewportScale);

        if (wheel > 0)
            m_ViewportScale *= 1.1f; // Zoom In
        else if (wheel < 0)
            m_ViewportScale *= 0.9f; // Zoom Out

        // Scale Limit
        if (m_ViewportScale < 0.1f)
            m_ViewportScale = 0.1f;

        if (m_ViewportScale > 100.f)
            m_ViewportScale = 100.f;

        // 마우스의 위치를 기준으로 확대/축소 후 뷰포트 오프셋 조정
        m_ViewportOffset.x = io.MousePos.x - mouse_pos_in_canvas_relative.x * m_ViewportScale - canvas_p0.x;
        m_ViewportOffset.y = io.MousePos.y - mouse_pos_in_canvas_relative.y * m_ViewportScale - canvas_p0.y;
    }

    ImGui::End();
}

void CTileMapEditor::DrawDetails()
{
    ImGui::Begin("Details##TileMapEditor");

    if (nullptr != m_TileMapObj && nullptr != m_TileMapObj->TileMap())
    {
        int TileCountX = m_TileMapObj->TileMap()->m_iTileCountX;
        int TileCountY = m_TileMapObj->TileMap()->m_iTileCountY;

        if (ImGui::InputInt(ImGuiLabelPrefix("Tile Count X").c_str(), &TileCountX, 1))
        {
            if (TileCountX < 0)
                TileCountX = 0;

            m_TileMapObj->TileMap()->SetTileCount(TileCountX, TileCountY);
        }

        if (ImGui::InputInt(ImGuiLabelPrefix("Tile Count Y").c_str(), &TileCountY, 1))
        {
            if (TileCountY < 0)
                TileCountY = 0;

            m_TileMapObj->TileMap()->SetTileCount(TileCountX, TileCountY);
        }

        ImGui::DragFloat2(ImGuiLabelPrefix("Tile Render Size").c_str(), &m_TileMapObj->TileMap()->m_vTileRenderSize.x,
                          1.f, 0, D3D11_FLOAT32_MAX);

        ImGui::DragFloat2(ImGuiLabelPrefix("Tile Pixel Size").c_str(), &m_TileMapObj->TileMap()->m_vTilePixelSize.x,
                          1.f, 0, D3D11_FLOAT32_MAX);
    }

    ImGui::End();
}

void CTileMapEditor::DrawTileSet()
{
    ImGui::Begin("TileSet##TileMapEditor");

    if (nullptr != m_TileMapObj && nullptr != m_TileMapObj->TileMap())
    {
        const map<wstring, Ptr<CAsset>>& mapTextures = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);
        vector<string> names;
        for (const auto& iter : mapTextures)
        {
            names.push_back(ToString(iter.first));
        }

        std::string CurTextureName;

        if (nullptr != m_TileMapObj->TileMap()->GetTileAtlas())
            CurTextureName = ToString(m_TileMapObj->TileMap()->GetTileAtlas()->GetKey());

        if (ImGuiComboUI(ImGuiLabelPrefix("Tile Sheet Texture").c_str(), CurTextureName, names))
        {
            m_TileMapObj->TileMap()->SetTileAtlas(CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(CurTextureName)),
                                                  m_TileMapObj->TileMap()->m_vTilePixelSize);
        }
    }

    ImGui::End();
}
