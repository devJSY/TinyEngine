#include "pch.h"
#include "CTileMapEditor.h"

CTileMapEditor::CTileMapEditor()
    : CEditor(EDITOR_TYPE::TILEMAP)
    , m_TileMap(nullptr)
    , m_ViewportOffset()
    , m_ViewportScale(1.f)
    , m_SelectedImgIdx(-1)
    , m_DrawMode(DRAW_MODE::PAINT)
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
    ImVec2 canvas_p1 = canvas_p0 + canvas_sz;

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz,
                           ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight); // Held
    const ImVec2 origin = canvas_p0 + m_ViewportOffset;
    const ImVec2 mouse_pos_in_canvas = io.MousePos - origin;

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    {
        m_ViewportOffset += io.MouseDelta;
    }

    if (nullptr != m_TileMap)
    {
        ImVec2 RenderSize = m_TileMap->m_vTileRenderSize;

        for (int y = 0; y < (int)m_TileMap->m_iTileCountY; y++)
        {
            for (int x = 0; x < (int)m_TileMap->m_iTileCountX; x++)
            {
                ImVec2 LT = ImVec2((float)x, (float)y) * RenderSize * m_ViewportScale;
                ImVec2 RB = LT + RenderSize * m_ViewportScale;

                int idx = m_TileMap->m_iTileCountX * y + x;

                // Image Render
                if (nullptr != m_TileMap->m_TileAtlas)
                {
                    if (m_TileMap->m_vecTileInfo[idx].bRender)
                    {
                        draw_list->AddImage((void*)m_TileMap->m_TileAtlas->GetSRV().Get(), origin + LT, origin + RB,
                                            m_TileMap->m_vecTileInfo[idx].vLeftTopUV,
                                            m_TileMap->m_vecTileInfo[idx].vLeftTopUV + m_TileMap->m_vSliceSizeUV);
                    }
                }

                // Border Render
                draw_list->AddRect(origin + LT, origin + RB, IM_COL32(255, 255, 255, 255));

                ImRect rect;
                rect.Min = origin + LT;
                rect.Max = origin + RB;

                if (rect.Contains(io.MousePos))
                {
                    // Hovering Border Render
                    draw_list->AddRect(origin + LT, origin + RB, IM_COL32(255, 0, 0, 255));

                    // Draw
                    if (m_DrawMode == DRAW_MODE::PAINT)
                    {
                        if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
                             ImGui::IsMouseDragging(ImGuiMouseButton_Left)) &&
                            m_SelectedImgIdx != -1)
                        {
                            m_TileMap->SetTileIndex(y, x, m_SelectedImgIdx);
                        }
                    }
                    else if (m_DrawMode == DRAW_MODE::ERASER)
                    {
                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
                            ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                        {
                            m_TileMap->m_vecTileInfo[idx].vLeftTopUV = Vec2();
                            m_TileMap->m_vecTileInfo[idx].bRender = false;
                            m_TileMap->m_vecTileInfo[idx].ImageIndex = -1;
                        }
                    }
                    else if (m_DrawMode == DRAW_MODE::FILL)
                    {
                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {
                            FillTileMap(x, y, m_TileMap->m_vecTileInfo[idx].ImageIndex);
                        }
                    }
                }
            }
        }
    }

    // =================================
    // Scale Set
    // =================================
    if (ImGui::IsWindowHovered())
    {
        float wheel = ImGui::GetIO().MouseWheel;

        // 마우스의 현재 위치를 캔버스 기준으로 계산
        ImVec2 mouse_pos_in_canvas_relative = (io.MousePos - canvas_p0 - m_ViewportOffset) / m_ViewportScale;

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
        m_ViewportOffset = io.MousePos - mouse_pos_in_canvas_relative * m_ViewportScale - canvas_p0;
    }

    ImGui::End();
}

void CTileMapEditor::DrawDetails()
{
    ImGui::Begin("Details##TileMapEditor");

    if (nullptr != m_TileMap)
    {
        int TileCountX = m_TileMap->m_iTileCountX;
        int TileCountY = m_TileMap->m_iTileCountY;

        if (ImGui::InputInt(ImGuiLabelPrefix("Tile Count X").c_str(), &TileCountX, 1))
        {
            if (TileCountX < 1)
                TileCountX = 1;

            m_TileMap->SetTileCount(TileCountX, TileCountY);
        }

        if (ImGui::InputInt(ImGuiLabelPrefix("Tile Count Y").c_str(), &TileCountY, 1))
        {
            if (TileCountY < 1)
                TileCountY = 1;

            m_TileMap->SetTileCount(TileCountX, TileCountY);
        }

        ImGui::DragFloat2(ImGuiLabelPrefix("Tile Render Size").c_str(), &m_TileMap->m_vTileRenderSize.x, 1.f, 0,
                          D3D11_FLOAT32_MAX);

        ImGui::Text("Draw Mode");
        ImGui::SameLine();
        ImGui::RadioButton("Paint", (int*)&m_DrawMode, (int)DRAW_MODE::PAINT);
        ImGui::SameLine();
        ImGui::RadioButton("Eraser", (int*)&m_DrawMode, (int)DRAW_MODE::ERASER);
        ImGui::SameLine();
        ImGui::RadioButton("Fill", (int*)&m_DrawMode, (int)DRAW_MODE::FILL);
    }

    ImGui::End();
}

void CTileMapEditor::DrawTileSet()
{
    ImGui::Begin("TileSet##TileMapEditor", 0, ImGuiWindowFlags_HorizontalScrollbar);

    if (nullptr != m_TileMap)
    {
        if (ImGui::DragFloat2(ImGuiLabelPrefix("Tile Pixel Size").c_str(), &m_TileMap->m_vTilePixelSize.x, 1.f, 0,
                              D3D11_FLOAT32_MAX))
        {
            if (nullptr != m_TileMap->m_TileAtlas)
                m_TileMap->SetTileAtlas(m_TileMap->m_TileAtlas, m_TileMap->m_vTilePixelSize);
        }

        const map<wstring, Ptr<CAsset>>& mapTextures = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);
        vector<string> names;
        for (const auto& iter : mapTextures)
        {
            names.push_back(ToString(iter.first));
        }

        std::string CurTextureName;

        if (nullptr != m_TileMap->GetTileAtlas())
            CurTextureName = ToString(m_TileMap->GetTileAtlas()->GetKey());

        if (ImGuiComboUI(ImGuiLabelPrefix("Tile Sheet Texture").c_str(), CurTextureName, names))
        {
            m_TileMap->SetTileAtlas(CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(CurTextureName)),
                                    m_TileMap->m_vTilePixelSize);
        }

        ImGui::Separator();

        if (nullptr != m_TileMap->GetTileAtlas() && m_TileMap->m_vTilePixelSize.x > 0 &&
            m_TileMap->m_vTilePixelSize.y > 0)
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            int col = int(m_TileMap->GetTileAtlas()->GetWidth() / m_TileMap->m_vTilePixelSize.x);
            int row = int(m_TileMap->GetTileAtlas()->GetHeight() / m_TileMap->m_vTilePixelSize.y);

            int TileCount = 0;
            float Widht = ImGui::GetContentRegionAvail().x;
            int NextLineCount = int(Widht / 64.f);

            for (int y = 0; y < row; y++)
            {
                for (int x = 0; x < col; x++)
                {
                    ImVec2 uv0 = ImVec2(m_TileMap->m_vSliceSizeUV.x * x, m_TileMap->m_vSliceSizeUV.y * y);
                    ImVec2 uv1 = uv0 + m_TileMap->m_vSliceSizeUV;

                    ImGui::Image((void*)m_TileMap->GetTileAtlas()->GetSRV().Get(), ImVec2(64.f, 64.f), uv0, uv1);

                    int ImageIdx = col * y + x;

                    ImU32 borderColor = IM_COL32(255, 255, 255, 255);
                    if (ImageIdx == m_SelectedImgIdx)
                        borderColor = IM_COL32(255, 0, 0, 255);

                    draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), borderColor);

                    if (ImGui::IsItemHovered())
                    {
                        draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));

                        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                        {
                            if (m_SelectedImgIdx == ImageIdx)
                                m_SelectedImgIdx = -1; // 선택 취소
                            else
                                m_SelectedImgIdx = ImageIdx; // 선택
                        }
                    }

                    TileCount += 1;
                    if (NextLineCount != TileCount)
                    {
                        ImGui::SameLine();
                    }
                    else
                    {
                        TileCount = 0;
                    }
                }
            }
        }
    }

    ImGui::End();
}

void CTileMapEditor::FillTileMap(int _startX, int _startY, int _TargetIdx)
{
    vector<vector<int>> visited(m_TileMap->m_iTileCountY, vector<int>(m_TileMap->m_iTileCountX));

    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    m_TileMap->SetTileIndex(_startY, _startX, m_SelectedImgIdx);

    std::queue<std::pair<int, int>> Q;

    Q.push({_startX, _startY});
    visited[_startY][_startX] = true;

    while (!Q.empty())
    {
        auto pos = Q.front();
        Q.pop();

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = pos.first + dx[dir];
            int ny = pos.second + dy[dir];

            if (nx < 0 || nx >= (int)m_TileMap->m_iTileCountX || ny < 0 || ny >= (int)m_TileMap->m_iTileCountY)
                continue;

            int idx = ny * m_TileMap->m_iTileCountX + nx;
            if (m_TileMap->m_vecTileInfo[idx].ImageIndex == _TargetIdx && !visited[ny][nx])
            {
                m_TileMap->SetTileIndex(ny, nx, m_SelectedImgIdx);
                Q.push({nx, ny});
                visited[ny][nx] = true;
            }
        }
    }
}
