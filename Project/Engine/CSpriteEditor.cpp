#include "pch.h"
#include "CSpriteEditor.h"
#include "CAssetMgr.h"

#include "CKeyMgr.h"

CSpriteEditor::CSpriteEditor()
    : CEditor(EDITOR_TYPE::SPRITE)
    , m_pTex()
    , m_Sprites{}
    , m_DragRect()
    , m_ViewportOffset()
    , m_ViewportScale(1.f)
    , m_LineCheckFlag(0)
    , m_vecFrm{}
    , m_CurAnimIdx(0)
    , m_AnimFPS(24)
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

    ImGuiSetWindowClass_SpriteEditor();
    DrawAnimationEdit();

    ImGuiSetWindowClass_SpriteEditor();
    DrawAnimationList();
}

void CSpriteEditor::DrawViewprot()
{
    ImGui::Begin("Viewprot##SpriteEditor");

    static bool opt_enable_grid = true;
    static bool Adding_Rect = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);

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

    /// -----------1------------
    /// |                      |
    /// |                      |
    /// |                      |
    /// |                      |
    /// |                      |
    /// 8      Line Check      2
    /// |                      |
    /// |                      |
    /// |                      |
    /// |                      |
    /// |                      |
    /// -----------4------------

    for (int i = 0; i < m_Sprites.Size; i++)
    {
        // Sprite Line Check
        if (ImGui::IsItemHovered())
        {
            ImVec2 mouse_pos = mouse_pos_in_canvas / m_ViewportScale;
            ImVec2 LeftDistVec =
                ImLineClosestPoint(m_Sprites[i].Rect.GetTL(), m_Sprites[i].Rect.GetBL(), mouse_pos) - mouse_pos;
            ImVec2 RightDistVec =
                ImLineClosestPoint(m_Sprites[i].Rect.GetTR(), m_Sprites[i].Rect.GetBR(), mouse_pos) - mouse_pos;
            ImVec2 TopDistVec =
                ImLineClosestPoint(m_Sprites[i].Rect.GetTL(), m_Sprites[i].Rect.GetTR(), mouse_pos) - mouse_pos;
            ImVec2 BottomDistVec =
                ImLineClosestPoint(m_Sprites[i].Rect.GetBL(), m_Sprites[i].Rect.GetBR(), mouse_pos) - mouse_pos;

            float LeftDistSquare = (LeftDistVec.x * LeftDistVec.x) + (LeftDistVec.y * LeftDistVec.y);
            float RightDist = (RightDistVec.x * RightDistVec.x) + (RightDistVec.y * RightDistVec.y);
            float TopDist = (TopDistVec.x * TopDistVec.x) + (TopDistVec.y * TopDistVec.y);
            float BottomDist = (BottomDistVec.x * BottomDistVec.x) + (BottomDistVec.y * BottomDistVec.y);

            float fDetectRange = 2.f;

            int LineFlag = 0;

            if (TopDist <= fDetectRange * fDetectRange)
            {
                LineFlag |= 1 << 0; // 1
            }
            if (RightDist <= fDetectRange * fDetectRange)
            {
                LineFlag |= 1 << 1; // 2
            }
            if (BottomDist <= fDetectRange * fDetectRange)
            {
                LineFlag |= 1 << 2; // 4
            }
            if (LeftDistSquare <= fDetectRange * fDetectRange)
            {
                LineFlag |= 1 << 3; // 8
            }

            int Top = 1;
            int Right = 2;
            int Bottom = 4;
            int Left = 8;

            // Mouse Cursor
            if (Left + Bottom == LineFlag || Right + Top == LineFlag)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNESW); // ⤢
            }
            else if (Left + Top == LineFlag || Right + Bottom == LineFlag)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE); // ⤡
            }
            else if (Top == LineFlag || Bottom == LineFlag)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS); // ⭥
            }
            else if (Left == LineFlag || Right == LineFlag)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW); // ↔
            }

            // 경계선을 좌클릭한 경우
            if (0 != LineFlag && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                m_LineCheckFlag = LineFlag;

                for (int j = 0; j < m_Sprites.Size; j++)
                    m_Sprites[j].bViewport_Selected = false;

                m_Sprites[i].bViewport_Selected = true;
                break;
            }

            // 좌클릭시 클릭한지점이 Sprite 내부에 존재한경우
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && m_Sprites[i].Rect.Contains(mouse_pos))
            {
                // 컨트롤키 누른상태만 다중선택
                if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
                    m_Sprites[i].bViewport_Selected = true;
                else
                {
                    for (int j = 0; j < m_Sprites.Size; j++)
                        m_Sprites[j].bViewport_Selected = false;

                    m_Sprites[i].bViewport_Selected = true;
                }

                break;
            }
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        for (int i = 0; i < m_Sprites.Size; i++)
        {
            if (!m_Sprites[i].bViewport_Selected)
                continue;

            // Min Max 확인
            if (m_Sprites[i].Rect.Max.x < m_Sprites[i].Rect.Min.x)
                std::swap(m_Sprites[i].Rect.Min.x, m_Sprites[i].Rect.Max.x);
            if (m_Sprites[i].Rect.Max.y < m_Sprites[i].Rect.Min.y)
                std::swap(m_Sprites[i].Rect.Min.y, m_Sprites[i].Rect.Max.y);

            break;
        }

        m_LineCheckFlag = 0;
    }

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        for (int i = 0; i < m_Sprites.Size; i++)
        {
            // 사각형 라인을 클릭한경우 크기 확장/ 축소
            if (0 != m_Sprites[i].bViewport_Selected && 0 != m_LineCheckFlag)
            {
                if ((1 << 0) & m_LineCheckFlag) // Top 1
                {
                    m_Sprites[i].Rect.Min.y += io.MouseDelta.y / m_ViewportScale;
                }
                if ((1 << 1) & m_LineCheckFlag) // Right 2
                {
                    m_Sprites[i].Rect.Max.x += io.MouseDelta.x / m_ViewportScale;
                }
                if ((1 << 2) & m_LineCheckFlag) // Bottom 4
                {
                    m_Sprites[i].Rect.Max.y += io.MouseDelta.y / m_ViewportScale;
                }
                if ((1 << 3) & m_LineCheckFlag) // Left 8
                {
                    m_Sprites[i].Rect.Min.x += io.MouseDelta.x / m_ViewportScale;
                }
            }
            // 사각형 내부를 클릭한 경우 전체 이동
            else if (0 != m_Sprites[i].bViewport_Selected)
            {
                m_Sprites[i].Rect.Min.x += io.MouseDelta.x / m_ViewportScale;
                m_Sprites[i].Rect.Min.y += io.MouseDelta.y / m_ViewportScale;

                m_Sprites[i].Rect.Max.x += io.MouseDelta.x / m_ViewportScale;
                m_Sprites[i].Rect.Max.y += io.MouseDelta.y / m_ViewportScale;
            }
        }
    }

    // Add first and second point
    if (ImGui::IsItemHovered() && !Adding_Rect && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && 0 == m_LineCheckFlag)
    {
        bool bDirtyFlag = false;

        // 선택중인 스프라이트 영역내를 클릭했는지 체크
        // 그 외의경우에만 Sprtie 새로 생성
        for (int i = 0; i < m_Sprites.Size; i++)
        {
            if (m_Sprites[i].bViewport_Selected && m_Sprites[i].Rect.Contains(mouse_pos_in_canvas / m_ViewportScale))
            {
                bDirtyFlag = true;
                break;
            }
        }

        if (!bDirtyFlag)
        {
            // 선택했던 스프라이트 해제
            for (int i = 0; i < m_Sprites.Size; i++)
            {
                m_Sprites[i].bViewport_Selected = false;
            }

            m_DragRect.Min = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) / m_ViewportScale;
            m_DragRect.Max = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) / m_ViewportScale;

            Adding_Rect = true;
        }
    }

    if (Adding_Rect)
    {
        m_DragRect.Max = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) / m_ViewportScale;
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            Adding_Rect = false;

            // Min Max 확인
            if (m_DragRect.Max.x < m_DragRect.Min.x)
                std::swap(m_DragRect.Min.x, m_DragRect.Max.x);
            if (m_DragRect.Max.y < m_DragRect.Min.y)
                std::swap(m_DragRect.Min.y, m_DragRect.Max.y);

            tSprite sprite;
            sprite.Rect = m_DragRect;
            sprite.bViewport_Selected = false;
            sprite.bSpriteList_Selected = false;
            m_Sprites.push_back(sprite);
        }
    }

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    {
        m_ViewportOffset.x += io.MouseDelta.x;
        m_ViewportOffset.y += io.MouseDelta.y;
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    if (ImGui::BeginPopup("context"))
    {
        if (Adding_Rect)
            m_Sprites.resize(m_Sprites.size() - 1);
        Adding_Rect = false;
        if (ImGui::MenuItem("Remove Selected Sprite", NULL, false, m_Sprites.Size > 0))
        {
            ImVector<tSprite>::iterator iter = m_Sprites.begin();

            for (; iter != m_Sprites.end();)
            {
                if ((*iter).bViewport_Selected)
                {
                    iter = m_Sprites.erase(iter);
                }
                else
                {
                    iter++;
                }
            }
        }
        if (ImGui::MenuItem("Remove Previous Sprite", NULL, false, m_Sprites.Size > 0))
        {
            m_Sprites.resize(m_Sprites.Size - 1);
        }
        if (ImGui::MenuItem("Remove all", NULL, false, m_Sprites.Size > 0))
        {
            m_Sprites.clear();
        }
        ImGui::EndPopup();
    }

    // =================================
    // Rendering
    // =================================

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = 32.0f;

        for (float x = fmodf(m_ViewportOffset.x, GRID_STEP * m_ViewportScale); x < canvas_sz.x;
             x += GRID_STEP * m_ViewportScale)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(m_ViewportOffset.y, GRID_STEP * m_ViewportScale); y < canvas_sz.y;
             y += GRID_STEP * m_ViewportScale)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
    }

    // Texture Render
    if (nullptr != m_pTex.Get())
    {
        ImVec2 TexSize = ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

        draw_list->AddImage((void*)m_pTex->GetSRV().Get(), origin, origin + TexSize * m_ViewportScale);
        draw_list->AddRect(origin, origin + TexSize * m_ViewportScale, IM_COL32(255, 255, 255, 255));
    }

    // Sprites Render
    for (int n = 0; n < m_Sprites.Size; n++)
    {
        ImVec2 min = ImVec2(origin.x + m_Sprites[n].Rect.Min.x * m_ViewportScale,
                            origin.y + m_Sprites[n].Rect.Min.y * m_ViewportScale);
        ImVec2 max = ImVec2(origin.x + m_Sprites[n].Rect.Max.x * m_ViewportScale,
                            origin.y + m_Sprites[n].Rect.Max.y * m_ViewportScale);

        if (m_Sprites[n].bViewport_Selected)
            draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
        else
            draw_list->AddRect(min, max, IM_COL32(0, 255, 0, 255));
    }

    // Rect Render
    if (Adding_Rect)
    {
        ImVec2 min =
            ImVec2(origin.x + m_DragRect.Min.x * m_ViewportScale, origin.y + m_DragRect.Min.y * m_ViewportScale);
        ImVec2 max =
            ImVec2(origin.x + m_DragRect.Max.x * m_ViewportScale, origin.y + m_DragRect.Max.y * m_ViewportScale);

        draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
    }

    draw_list->PopClipRect();

    // Scale
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
    ImGui::Begin("Sprite List##SpriteEditor", 0, ImGuiWindowFlags_HorizontalScrollbar);

    if (nullptr != m_pTex.Get())
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        for (int i = 1; i <= m_Sprites.Size; i++)
        {
            int idx = i - 1;
            ImVec2 TextureSize = ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

            ImU32 borderColor = IM_COL32(255, 255, 255, 255);

            if (m_Sprites[idx].bSpriteList_Selected)
                borderColor = IM_COL32(255, 0, 0, 255);

            ImGui::ImageButton((void*)m_pTex->GetSRV().Get(), ImVec2(100.f, 100.f),
                               m_Sprites[idx].Rect.Min / TextureSize, m_Sprites[idx].Rect.Max / TextureSize);

            draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), borderColor);

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                ImRect rect;
                rect.Min = ImGui::GetItemRectMin();
                rect.Max = ImGui::GetItemRectMax();
                if (rect.Contains(ImGui::GetIO().MousePos))
                {
                    m_Sprites[idx].bSpriteList_Selected = !m_Sprites[idx].bSpriteList_Selected;
                }
            }

            float Widht = ImGui::GetContentRegionAvail().x;
            int col = (int)Widht / 100;
            if (col <= 0)
                col = 1;

            if (i % col != 0)
                ImGui::SameLine();
        }
    }

    // Create Animation
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        ImGui::OpenPopup("CreateAnimation_popup");

    if (ImGui::BeginPopup("CreateAnimation_popup"))
    {
        if (ImGui::MenuItem("Create Animation##SpriteList", NULL, false, m_Sprites.Size > 0))
        {
            m_vecFrm.clear();
            for (int i = 0; i < m_Sprites.Size; i++)
            {
                if (m_Sprites[i].bSpriteList_Selected)
                {
                    tAnimData2D AnimData;

                    AnimData.vLeftTop = Vec2(m_Sprites[i].Rect.Min.x, m_Sprites[i].Rect.Min.y);
                    AnimData.vSliceSize = Vec2(m_Sprites[i].Rect.GetSize().x, m_Sprites[i].Rect.GetSize().y);
                    AnimData.vBackGround = Vec2();
                    AnimData.vOffset = Vec2();

                    m_vecFrm.push_back(AnimData);
                }

                m_Sprites[i].bSpriteList_Selected = false;
            }
        }

        ImGui::EndPopup();
    }
    ImGui::End();
}

void CSpriteEditor::DrawAnimationEdit()
{
    ImGui::Begin("Animation Edit##SpriteEditor");

    ImVec2 canvas_LT = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
    if (canvas_sz.x < 50.0f)
        canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f)
        canvas_sz.y = 50.0f;
    ImVec2 canvas_RB = ImVec2(canvas_LT.x + canvas_sz.x, canvas_LT.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_LT, canvas_RB, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_LT, canvas_RB, IM_COL32(255, 255, 255, 255));

    draw_list->AddLine(ImVec2(canvas_LT.x, canvas_LT.y + (canvas_sz.y / 2.f)),
                       ImVec2(canvas_RB.x, canvas_LT.y + (canvas_sz.y / 2.f)), IM_COL32(255, 0, 0, 255)); // 가로 축
    draw_list->AddLine(ImVec2(canvas_LT.x + (canvas_sz.x / 2.f), canvas_LT.y),
                       ImVec2(canvas_LT.x + (canvas_sz.x / 2.f), canvas_RB.y), IM_COL32(0, 0, 255, 255)); // 세로 축

    if (nullptr != m_pTex.Get() && m_vecFrm.size() > 0)
    {
        ImVec2 vTextureSize = ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

        ImVec2 RenderSize = ImVec2(500.f, 500.f);
        ImVec2 vLT = canvas_LT + (canvas_sz / 2.f) - (RenderSize / 2.f);

        ImVec2 uv0 = ImVec2(m_vecFrm[m_CurAnimIdx].vLeftTop.x / vTextureSize.x,
                            m_vecFrm[m_CurAnimIdx].vLeftTop.y / vTextureSize.y);

        ImVec2 uv1 = ImVec2((m_vecFrm[m_CurAnimIdx].vLeftTop.x + m_vecFrm[m_CurAnimIdx].vSliceSize.x) / vTextureSize.x,
                            (m_vecFrm[m_CurAnimIdx].vLeftTop.y + m_vecFrm[m_CurAnimIdx].vSliceSize.y) / vTextureSize.y);
        draw_list->AddImage((void*)m_pTex->GetSRV().Get(), vLT, vLT + RenderSize, uv0, uv1);
    }

    ImGui::End();
}

void CSpriteEditor::DrawAnimationList()
{
    ImGui::Begin("Animation List##SpriteEditor", 0, ImGuiWindowFlags_HorizontalScrollbar);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (UINT i = 0; i < m_vecFrm.size(); i++)
    {
        Vec2 TextureSize = Vec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

        ImGui::Image((void*)m_pTex->GetSRV().Get(), ImVec2(100.f, 100.f),
                     ImVec2((m_vecFrm[i].vLeftTop / TextureSize).x, (m_vecFrm[i].vLeftTop / TextureSize).y),
                     ImVec2(((m_vecFrm[i].vLeftTop + m_vecFrm[i].vSliceSize) / TextureSize).x,
                            ((m_vecFrm[i].vLeftTop + m_vecFrm[i].vSliceSize) / TextureSize).y));

        draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 255));

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
