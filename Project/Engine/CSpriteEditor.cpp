#include "pch.h"
#include "CSpriteEditor.h"
#include "CAssetMgr.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CSpriteEditor::CSpriteEditor()
    : CEditor(EDITOR_TYPE::SPRITE)
    , m_pTex()
    , m_Sprites{}
    , m_DragRect()
    , m_ViewportOffset()
    , m_ViewportScale(1.f)
    , m_LineCheckFlag(0)
    , m_pAnim(nullptr)
    , m_AnimFPS(12)
    , m_bAnimPlay(true)
    , m_CellWidth(0)
    , m_CellHeight(0)

{
}

CSpriteEditor::~CSpriteEditor()
{
    if (nullptr != m_pAnim)
    {
        delete m_pAnim;
        m_pAnim = nullptr;
    }
}

void CSpriteEditor::init()
{
}

void CSpriteEditor::render()
{
    ImGui_SetWindowClass(GetEditorType());
    DrawViewport();

    ImGui_SetWindowClass(GetEditorType());
    DrawDetails();

    ImGui_SetWindowClass(GetEditorType());
    DrawSpriteList();

    ImGui_SetWindowClass(GetEditorType());
    DrawAnimationViewport();

    ImGui_SetWindowClass(GetEditorType());
    DrawAnimationList();
}

void CSpriteEditor::DrawViewport()
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
    ImVec2 canvas_p1 = canvas_p0 + canvas_sz;

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz,
                           ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight); // Held
    const ImVec2 origin = canvas_p0 + m_ViewportOffset;
    const ImVec2 mouse_pos_in_canvas = io.MousePos - origin;

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
            ImVec2 LeftDistVec = ImLineClosestPoint(m_Sprites[i].Rect.GetTL(), m_Sprites[i].Rect.GetBL(), mouse_pos) - mouse_pos;
            ImVec2 RightDistVec = ImLineClosestPoint(m_Sprites[i].Rect.GetTR(), m_Sprites[i].Rect.GetBR(), mouse_pos) - mouse_pos;
            ImVec2 TopDistVec = ImLineClosestPoint(m_Sprites[i].Rect.GetTL(), m_Sprites[i].Rect.GetTR(), mouse_pos) - mouse_pos;
            ImVec2 BottomDistVec = ImLineClosestPoint(m_Sprites[i].Rect.GetBL(), m_Sprites[i].Rect.GetBR(), mouse_pos) - mouse_pos;

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

                // 우클릭 화면 이동 Offset
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
                {
                    if ((1 << 0) & m_LineCheckFlag) // Top 1
                    {
                        m_Sprites[i].Rect.Min.y -= io.MouseDelta.y / m_ViewportScale;
                    }
                    if ((1 << 1) & m_LineCheckFlag) // Right 2
                    {
                        m_Sprites[i].Rect.Max.x -= io.MouseDelta.x / m_ViewportScale;
                    }
                    if ((1 << 2) & m_LineCheckFlag) // Bottom 4
                    {
                        m_Sprites[i].Rect.Max.y -= io.MouseDelta.y / m_ViewportScale;
                    }
                    if ((1 << 3) & m_LineCheckFlag) // Left 8
                    {
                        m_Sprites[i].Rect.Min.x -= io.MouseDelta.x / m_ViewportScale;
                    }
                }
            }
            // 사각형 내부를 클릭한 경우 전체 이동
            else if (0 != m_Sprites[i].bViewport_Selected)
            {
                m_Sprites[i].Rect.Min += io.MouseDelta / m_ViewportScale;
                m_Sprites[i].Rect.Max += io.MouseDelta / m_ViewportScale;
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

            m_DragRect.Min = mouse_pos_in_canvas / m_ViewportScale;
            m_DragRect.Max = mouse_pos_in_canvas / m_ViewportScale;

            Adding_Rect = true;
        }
    }

    if (Adding_Rect)
    {
        m_DragRect.Max = mouse_pos_in_canvas / m_ViewportScale;
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

    // 우클릭 화면 이동
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

        if (ImGui::MenuItem("Align Selected Sprites", NULL, false, m_Sprites.Size > 0))
        {
            ImVector<tSprite>::iterator iter = m_Sprites.begin();

            for (; iter != m_Sprites.end();)
            {
                // 선택된 Sprite 중에서 정렬 성공한 경우 기존 Sprite 삭제
                if ((*iter).bViewport_Selected)
                {
                    if (AlignSprite(*iter))
                    {
                        iter = m_Sprites.erase(iter);
                    }
                    else
                    {
                        iter++;
                    }
                }
                else
                {
                    iter++;
                }
            }
        }
        if (ImGui::MenuItem("Delete Selected Sprites", NULL, false, m_Sprites.Size > 0))
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
        if (ImGui::MenuItem("Delete Previous Sprite", NULL, false, m_Sprites.Size > 0))
        {
            m_Sprites.resize(m_Sprites.Size - 1);
        }
        if (ImGui::MenuItem("Delete all Sprites", NULL, false, m_Sprites.Size > 0))
        {
            m_Sprites.clear();
        }
        if (ImGui::MenuItem("Create Animation", NULL, false, m_Sprites.Size > 0))
        {
            if (nullptr != m_pAnim)
                delete m_pAnim;

            m_pAnim = new CAnim;
            m_pAnim->m_AtlasTex = m_pTex;

            for (int i = 0; i < m_Sprites.Size; i++)
            {
                ImVec2 vTextureSize = ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

                if (m_Sprites[i].bViewport_Selected)
                {
                    tAnimFrm AnimData;

                    // UV 좌표로 변환
                    AnimData.vLeftTop = m_Sprites[i].Rect.GetTL() / vTextureSize;
                    AnimData.vSlice = m_Sprites[i].Rect.GetSize() / vTextureSize;
                    AnimData.vOffset = Vec2(0.f, 0.f);
                    AnimData.vBackground = Vec2(0.f, 0.f);
                    AnimData.Duration = 1.f / m_AnimFPS;

                    m_pAnim->m_vecFrm.push_back(AnimData);
                }
            }

            // 선택된 Sprite가 없는 상태에서 애니메이션 생성요청했을경우
            if (m_pAnim->m_vecFrm.empty())
            {
                delete m_pAnim;
                m_pAnim = nullptr;
            }
        }

        ImGui::EndPopup();
    }

    // =================================
    // Rendering
    // =================================

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true); // 캔버스 영역내 렌더링 제한
    if (opt_enable_grid)
    {
        const float GRID_STEP = 32.0f;

        for (float x = fmodf(m_ViewportOffset.x, GRID_STEP * m_ViewportScale); x < canvas_sz.x; x += GRID_STEP * m_ViewportScale)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(m_ViewportOffset.y, GRID_STEP * m_ViewportScale); y < canvas_sz.y; y += GRID_STEP * m_ViewportScale)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
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
        ImVec2 min = origin + (m_Sprites[n].Rect.Min * m_ViewportScale);
        ImVec2 max = origin + (m_Sprites[n].Rect.Max * m_ViewportScale);

        if (m_Sprites[n].bViewport_Selected)
            draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
        else
            draw_list->AddRect(min, max, IM_COL32(0, 255, 0, 255));
    }

    // Rect Render
    if (Adding_Rect)
    {
        ImVec2 min = origin + (m_DragRect.Min * m_ViewportScale);
        ImVec2 max = origin + (m_DragRect.Max * m_ViewportScale);

        draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
    }

    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255)); // canvas 테두리
    draw_list->PopClipRect();

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

void CSpriteEditor::DrawDetails()
{
    ImGui::Begin("Details##SpriteEditor");

    ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                             ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    // ==========================
    // Sprites
    // ==========================
    if (ImGui::TreeNodeEx("Sprites##SpriteEditorDetails", DefaultTreeNodeFlag))
    {
        static std::string CurTextureName;

        if (nullptr != m_pTex.Get())
            CurTextureName = ToString(m_pTex->GetKey());

        if (ImGui_TexturesComboUI(ImGui_LabelPrefix("Source Texture").c_str(), CurTextureName))
        {
            m_pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(CurTextureName));
            m_CellWidth = m_pTex->GetWidth();
            m_CellHeight = m_pTex->GetHeight();
            m_Sprites.clear();
        }

        if (nullptr != m_pTex.Get())
        {
            ImGui::Image((void*)m_pTex->GetSRV().Get(), ImVec2(250.f, 250.f));
        }

        ImGui::TreePop();
    }

    // ==========================
    // Extract sprites
    // ==========================
    if (nullptr != m_pTex.Get())
    {
        if (ImGui::TreeNodeEx("Extract sprites##SpriteEditorDetails", DefaultTreeNodeFlag))
        {
            static vector<string> spriteExtractModes = {"Auto", "Grid"};
            static string CurMode = spriteExtractModes[0];

            ImGui_ComboUI(ImGui_LabelPrefix("Sprite Extract Mode").c_str(), CurMode, spriteExtractModes);

            // Grid option
            if (spriteExtractModes[1] == CurMode)
            {
                ImGui::DragInt(ImGui_LabelPrefix("Cell Width").c_str(), &m_CellWidth, 1.f, 0, m_pTex->GetWidth());
                ImGui::DragInt(ImGui_LabelPrefix("Cell Height").c_str(), &m_CellHeight, 1.f, 0, m_pTex->GetHeight());
            }

            if (ImGui::Button("Extract..."))
            {
                // Auto
                if (spriteExtractModes[0] == CurMode)
                {
                    tPixel* pPixel = m_pTex->GetPixels();

                    for (int y = 0; y < (int)m_pTex->GetHeight(); y++)
                    {
                        for (int x = 0; x < (int)m_pTex->GetWidth(); x++)
                        {
                            int idx = (m_pTex->GetWidth() * y) + x;

                            if (pPixel[idx].a == 0)
                                continue;

                            ExtractSprite(pPixel, x, y, m_pTex->GetWidth(), m_pTex->GetHeight());
                        }
                    }

                    // 원본 이미지 GPU에서 불러와서 데이터 재설정
                    m_pTex->CaptureTex();
                }
                // Grid
                else
                {
                    int WidhtCount = m_pTex->GetWidth() / m_CellWidth;
                    int HeightCount = m_pTex->GetHeight() / m_CellHeight;

                    for (int y = 0; y < HeightCount; y++)
                    {
                        for (int x = 0; x < WidhtCount; x++)
                        {
                            ImRect rect;

                            rect.Min.x = float(m_CellWidth * x);
                            rect.Min.y = float(m_CellHeight * y);
                            rect.Max.x = float(m_CellWidth * x + m_CellWidth);
                            rect.Max.y = float(m_CellHeight * y + m_CellHeight);

                            // 비어있는 영역인지 체크
                            bool bEmpty = true;
                            tPixel* pPixel = m_pTex->GetPixels();

                            for (int y = (int)rect.Min.y; y <= (int)rect.Max.y; y++)
                            {
                                for (int x = (int)rect.Min.x; x <= (int)rect.Max.x; x++)
                                {
                                    int idx = (m_pTex->GetWidth() * y) + x;

                                    if (pPixel[idx].a != 0)
                                    {
                                        bEmpty = false;
                                        break;
                                    }
                                }

                                if (!bEmpty)
                                    break;
                            }

                            // 비어있지 않는 영역만 생성
                            if (!bEmpty)
                            {
                                tSprite sprite;
                                sprite.Rect = rect;
                                sprite.bViewport_Selected = false;
                                sprite.bSpriteList_Selected = false;

                                m_Sprites.push_back(sprite);
                            }
                        }
                    }
                }
            }

            ImGui::TreePop();
        }
    }

    // ==========================
    // Animation
    // ==========================
    if (ImGui::TreeNodeEx("Animation##SpriteEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_pAnim)
        {
            string StopPlay;

            if (m_bAnimPlay)
                StopPlay = "Animation Stop";
            else
                StopPlay = "Animation Play";

            if (ImGui::Button(StopPlay.c_str()))
            {
                m_bAnimPlay = !m_bAnimPlay;
            }

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));

            string name = ToString(m_pAnim->GetName());
            strcpy_s(buffer, sizeof(buffer), name.c_str());
            if (ImGui::InputText(ImGui_LabelPrefix("Animation Name").c_str(), buffer, sizeof(buffer)))
            {
                m_pAnim->SetName(ToWstring(buffer));
            }

            if (ImGui::InputInt(ImGui_LabelPrefix("FPS").c_str(), &m_AnimFPS))
            {
                if (m_AnimFPS < 1)
                    m_AnimFPS = 1;

                for (size_t i = 0; i < m_pAnim->m_vecFrm.size(); i++)
                {
                    m_pAnim->m_vecFrm[i].Duration = 1.f / m_AnimFPS;
                }
            }

            ImGui::SliderInt(ImGui_LabelPrefix("Frame Index").c_str(), &m_pAnim->m_CurFrmIdx, 0, (int)m_pAnim->m_vecFrm.size() - 1);

            ImGui::DragFloat(ImGui_LabelPrefix("Animation Offset X").c_str(), &m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset.x);

            ImGui::DragFloat(ImGui_LabelPrefix("Animation Offset Y").c_str(), &m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset.y);

            ImGui::Text("Animation Offset");
            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150);
            if (ImGui::ArrowButton("##left", ImGuiDir_Left))
                m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset.x -= 1.f;

            ImGui::SameLine();
            if (ImGui::ArrowButton("##Right", ImGuiDir_Right))
                m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset.x += 1.f;

            ImGui::SameLine();
            if (ImGui::ArrowButton("##Up", ImGuiDir_Up))
                m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset.y += 1.f;

            ImGui::SameLine();
            if (ImGui::ArrowButton("##Down", ImGuiDir_Down))
                m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset.y -= 1.f;

            if (ImGui::DragFloat2(ImGui_LabelPrefix("Animation BackGround").c_str(), &m_vAnimBackGround.x, 1.f))
            {
                for (size_t i = 0; i < m_pAnim->m_vecFrm.size(); i++)
                {
                    m_pAnim->m_vecFrm[i].vBackground = m_vAnimBackGround;
                }
            }

            ImGui::Checkbox(ImGui_LabelPrefix("Use BackGround").c_str(), &m_pAnim->m_bUseBackGround);

            // Atlas Texture
            Ptr<CTexture> pAtlas = m_pAnim->GetAtlasTex();
            if (nullptr != pAtlas)
            {
                ID3D11ShaderResourceView* pSRV = nullptr;
                pSRV = pAtlas->GetSRV().Get();
                ImGui_InputText("Atlas Texture", ToString(pAtlas->GetKey()));
                ImGui::Image((void*)pSRV, ImVec2((float)pAtlas->GetWidth(), (float)pAtlas->GetHeight()));
            }

            ImGui::Separator();
        }

        if (ImGui::Button("Load Animation"))
        {
            std::filesystem::path filePath = OpenFileDialog(L"AnimData\\", TEXT("애니메이션 파일\0*.anim\0모든 파일(*.*)\0*.*\0"));

            if (!filePath.empty()) // 취소, 닫기 버튼 체크
            {
                if (nullptr != m_pAnim)
                    delete m_pAnim;

                m_pAnim = new CAnim;
                m_pAnim->LoadAnim(filePath);

                // UV to Pixel
                for (size_t i = 0; i < m_pAnim->m_vecFrm.size(); i++)
                {
                    m_pAnim->m_vecFrm[i].Duration = 1.f / m_AnimFPS;
                    m_pAnim->m_vecFrm[i].vOffset.x *= (float)m_pAnim->GetAtlasTex()->GetWidth();
                    m_pAnim->m_vecFrm[i].vOffset.y *= (float)m_pAnim->GetAtlasTex()->GetHeight();
                    m_vAnimBackGround.x = m_pAnim->m_vecFrm[i].vBackground.x * (float)m_pAnim->GetAtlasTex()->GetWidth();
                    m_vAnimBackGround.y = m_pAnim->m_vecFrm[i].vBackground.y * (float)m_pAnim->GetAtlasTex()->GetHeight();
                }
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Save Animation"))
        {
            std::filesystem::path filePath = SaveFileDialog(L"AnimData\\", TEXT("애니메이션 파일\0*.anim\0모든 파일(*.*)\0*.*\0"));

            if (!filePath.empty()) // 취소, 닫기 버튼 체크
            {
                if (".anim" != filePath.extension())
                    filePath.replace_extension(".anim");

                if (nullptr != m_pAnim)
                {
                    // Pixel to UV
                    for (size_t i = 0; i < m_pAnim->m_vecFrm.size(); i++)
                    {
                        m_pAnim->m_vecFrm[i].Duration = 1.f / m_AnimFPS;
                        m_pAnim->m_vecFrm[i].vOffset.x /= (float)m_pAnim->GetAtlasTex()->GetWidth();
                        m_pAnim->m_vecFrm[i].vOffset.y /= (float)m_pAnim->GetAtlasTex()->GetHeight();
                        m_pAnim->m_vecFrm[i].vBackground.x = m_vAnimBackGround.x / (float)m_pAnim->GetAtlasTex()->GetWidth();
                        m_pAnim->m_vecFrm[i].vBackground.y = m_vAnimBackGround.y / (float)m_pAnim->GetAtlasTex()->GetHeight();
                    }

                    if (m_pAnim->GetName().empty())
                        m_pAnim->SetName(filePath.stem());

                    m_pAnim->SaveAnim(filePath);

                    // UV to Pixel
                    for (size_t i = 0; i < m_pAnim->m_vecFrm.size(); i++)
                    {
                        m_pAnim->m_vecFrm[i].vOffset.x *= (float)m_pAnim->GetAtlasTex()->GetWidth();
                        m_pAnim->m_vecFrm[i].vOffset.y *= (float)m_pAnim->GetAtlasTex()->GetHeight();
                        m_vAnimBackGround.x = m_pAnim->m_vecFrm[i].vBackground.x * (float)m_pAnim->GetAtlasTex()->GetWidth();
                        m_vAnimBackGround.y = m_pAnim->m_vecFrm[i].vBackground.y * (float)m_pAnim->GetAtlasTex()->GetHeight();
                    }
                }
            }
        }

        ImGui::TreePop();
    }

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

            ImGui::ImageButton((void*)m_pTex->GetSRV().Get(), ImVec2(100.f, 100.f), m_Sprites[idx].Rect.Min / TextureSize,
                               m_Sprites[idx].Rect.Max / TextureSize);

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
            if (nullptr != m_pAnim)
                delete m_pAnim;

            m_pAnim = new CAnim;
            m_pAnim->m_AtlasTex = m_pTex;

            for (int i = 0; i < m_Sprites.Size; i++)
            {
                ImVec2 vTextureSize = ImVec2((float)m_pTex->GetWidth(), (float)m_pTex->GetHeight());

                if (m_Sprites[i].bSpriteList_Selected)
                {
                    tAnimFrm AnimData;

                    // UV 좌표로 변환
                    AnimData.vLeftTop = m_Sprites[i].Rect.GetTL() / vTextureSize;
                    AnimData.vSlice = m_Sprites[i].Rect.GetSize() / vTextureSize;
                    AnimData.vOffset = Vec2(0.f, 0.f);
                    AnimData.vBackground = Vec2(0.f, 0.f);
                    AnimData.Duration = 1.f / m_AnimFPS;

                    m_pAnim->m_vecFrm.push_back(AnimData);
                }

                m_Sprites[i].bSpriteList_Selected = false;
            }

            // 선택된 Sprite가 없는 상태에서 애니메이션 생성요청했을경우
            if (m_pAnim->m_vecFrm.empty())
            {
                delete m_pAnim;
                m_pAnim = nullptr;
            }
        }

        ImGui::EndPopup();
    }
    ImGui::End();
}

void CSpriteEditor::DrawAnimationViewport()
{
    ImGui::Begin("Animation Viewport##SpriteEditor");

    ImVec2 canvas_LT = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
    if (canvas_sz.x < 50.0f)
        canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f)
        canvas_sz.y = 50.0f;
    ImVec2 canvas_RB = canvas_LT + canvas_sz;

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_LT, canvas_RB, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_LT, canvas_RB, IM_COL32(255, 255, 255, 255));

    draw_list->AddLine(ImVec2(canvas_LT.x, canvas_LT.y + (canvas_sz.y / 2.f)), ImVec2(canvas_RB.x, canvas_LT.y + (canvas_sz.y / 2.f)),
                       IM_COL32(255, 0, 0, 255)); // 가로 축
    draw_list->AddLine(ImVec2(canvas_LT.x + (canvas_sz.x / 2.f), canvas_LT.y), ImVec2(canvas_LT.x + (canvas_sz.x / 2.f), canvas_RB.y),
                       IM_COL32(0, 0, 255, 255)); // 세로 축

    if (nullptr != m_pAnim)
    {
        ImVec2 RenderSize = ImVec2(350.f, 350.f);
        ImVec2 vLT = canvas_LT + (canvas_sz / 2.f) - (RenderSize / 2.f);
        ImVec2 vOffset = m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vOffset;
        vLT.x += vOffset.x;
        vLT.y += vOffset.y;
        ImVec2 uv0 = m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vLeftTop;
        ImVec2 uv1 = m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vLeftTop + m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].vSlice;

        draw_list->AddImage((void*)m_pAnim->GetAtlasTex()->GetSRV().Get(), vLT, vLT + RenderSize, uv0, uv1);
    }

    ImGui::End();
}

void CSpriteEditor::DrawAnimationList()
{
    ImGui::Begin("Animation List##SpriteEditor", 0, ImGuiWindowFlags_HorizontalScrollbar);

    if (nullptr != m_pAnim)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        for (UINT i = 0; i < m_pAnim->m_vecFrm.size(); i++)
        {
            ImGui::Image((void*)m_pAnim->GetAtlasTex()->GetSRV().Get(), ImVec2(100.f, 100.f), m_pAnim->m_vecFrm[i].vLeftTop,
                         m_pAnim->m_vecFrm[i].vLeftTop + m_pAnim->m_vecFrm[i].vSlice);

            // Drag & Drop
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::Image((void*)m_pAnim->GetAtlasTex()->GetSRV().Get(), ImVec2(100.f, 100.f), m_pAnim->m_vecFrm[i].vLeftTop,
                             m_pAnim->m_vecFrm[i].vLeftTop + m_pAnim->m_vecFrm[i].vSlice);

                ImGui::SetDragDropPayload("ANIMATION_LIST_SPRITE", &i, sizeof(int));
                ImGui::EndDragDropSource();
            }

            // Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ANIMATION_LIST_SPRITE"))
                {
                    int idx = *(const int*)payload->Data;
                    std::swap(m_pAnim->m_vecFrm[i], m_pAnim->m_vecFrm[idx]);
                    m_pAnim->m_CurFrmIdx = i;
                }

                ImGui::EndDragDropTarget();
            }

            ImU32 borderColor = IM_COL32(255, 255, 255, 255);
            if (i == m_pAnim->m_CurFrmIdx)
                borderColor = IM_COL32(255, 0, 0, 255);

            draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), borderColor);

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                m_pAnim->m_CurFrmIdx = i;
            }

            ImGui::SameLine();
        }
    }

    ImGui::End();
}

void CSpriteEditor::finaltick()
{
    if (nullptr == m_pAnim || !m_bAnimPlay)
        return;

    m_pAnim->m_fAccTime += DT_ENGINE;

    if (m_pAnim->m_vecFrm[m_pAnim->m_CurFrmIdx].Duration < m_pAnim->m_fAccTime)
    {
        ++m_pAnim->m_CurFrmIdx;
        if (m_pAnim->m_vecFrm.size() <= m_pAnim->m_CurFrmIdx)
        {
            m_pAnim->m_CurFrmIdx = 0;
        }
        m_pAnim->m_fAccTime = 0.f;
    }
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

int CSpriteEditor::ExtractSprite(tPixel* pPixel, int _x, int _y, int _width, int _height)
{
    // Right → Bottom → Left → Top
    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    ImRect rect;
    rect.Min.x = (float)_x;
    rect.Min.y = (float)_y;
    rect.Max.x = (float)_x;
    rect.Max.y = (float)_y;

    std::queue<std::pair<int, int>> Q;

    Q.push({_x, _y});
    pPixel[_y * _width + _x].a = 0;

    while (!Q.empty())
    {
        auto pos = Q.front();
        Q.pop();

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = pos.first + dx[dir];
            int ny = pos.second + dy[dir];

            // 범위 체크
            if (nx >= 0 && nx < _width && ny >= 0 && ny < _height)
            {
                int newIdx = ny * _width + nx;

                if (0 == pPixel[newIdx].a)
                    continue;

                if (rect.Min.x >= nx)
                    rect.Min.x = (float)nx;
                if (rect.Min.y >= ny)
                    rect.Min.y = (float)ny;
                if (rect.Max.x <= nx)
                    rect.Max.x = (float)nx;
                if (rect.Max.y <= ny)
                    rect.Max.y = (float)ny;

                Q.push({nx, ny});
                pPixel[newIdx].a = 0;
            }
        }
    }

    // 최소 크기 제한
    if (rect.GetArea() > 3.f)
    {
        float padding = 1.f;

        rect.Min.x -= padding;
        rect.Min.y -= padding;
        rect.Max.x += padding;
        rect.Max.y += padding;

        tSprite sprite;
        sprite.Rect = rect;
        sprite.bViewport_Selected = false;
        sprite.bSpriteList_Selected = false;

        m_Sprites.push_back(sprite);
        return true;
    }

    return false;
}

int CSpriteEditor::AlignSprite(tSprite& _sprite)
{
    if (nullptr == m_pTex)
        return false;

    tPixel* pPixel = m_pTex->GetPixels();
    int texWidth = m_pTex->GetWidth();
    int texHeight = m_pTex->GetHeight();

    for (int y = (int)_sprite.Rect.Min.y; y <= (int)_sprite.Rect.Max.y; y++)
    {
        for (int x = (int)_sprite.Rect.Min.x; x <= (int)_sprite.Rect.Max.x; x++)
        {
            // 텍스춰의 범위를 벗어나는 경우
            if (x < 0 || x >= texWidth || y < 0 || y >= texHeight)
            {
                return false;
            }

            int idx = (m_pTex->GetWidth() * y) + x;

            if (pPixel[idx].a == 0)
                continue;

            int result = ExtractSprite(pPixel, x, y, texWidth, texHeight);
            m_pTex->CaptureTex(); // 원본 이미지 GPU에서 불러와서 데이터 재설정

            if (result)
                return true;
            else
                return false;
        }
    }

    return false;
}
