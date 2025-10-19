#include "pch.h"

#include "CEditorMgr.h"
#include "func.h"

void ImGui_SetWindowClass(EDITOR_TYPE _Type)
{
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID(ToString(CEditorMgr::GetInst()->GetEditor(_Type)->GetName()).c_str());
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);
}

bool ImGui_DrawVec3Control(const string& label, Vec3& values, float speed, float min, float max, float resetValue, float columnWidth)
{
    bool bDirtyflag = false;

    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 4.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.f);

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize))
    {
        values.x = resetValue;
        bDirtyflag = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f"))
    {
        bDirtyflag = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
    {
        values.y = resetValue;
        bDirtyflag = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f"))
    {
        bDirtyflag = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
    {
        values.z = resetValue;
        bDirtyflag = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Z", &values.z, speed, min, max, "%.2f"))
    {
        bDirtyflag = true;
    }
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    return bDirtyflag;
}

string ImGui_LabelPrefix(const char* const label, float alignment)
{
    float width = ImGui::CalcItemWidth();

    float x = ImGui::GetCursorPosX();
    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::SetCursorPosX(x + width * alignment + ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetNextItemWidth(-1);

    string labelID = "##";
    labelID += label;

    return labelID;
}

//
// Combo with std::vector<string>
//
bool ImGui_ComboUI(const string& caption, string& current_item, const std::vector<string>& items)
{
    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
    {
        for (int n = 0; n < items.size(); n++)
        {
            bool is_selected = (current_item == items[n]);

            if (ImGui::Selectable(items[n].c_str(), is_selected))
            {
                current_item = items[n];
                changed = true;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

bool ImGui_TexturesComboUI(const string& caption, string& current_item)
{
    static ImGuiTextFilter filter;
    const map<wstring, Ptr<CAsset>>& mapTextures = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);

    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
    {
        filter.Draw(ImGui_LabelPrefix("Filter").c_str());
        ImGui::Separator();

        if (ImGui::Selectable("...", false))
        {
            current_item = string();
            changed = true;
        }

        for (const auto& iter : mapTextures)
        {
            string key = ToString(iter.first);
            bool is_selected = (current_item == key);

            if (!filter.PassFilter(key.c_str()))
                continue;

            if (ImGui::Selectable(key.c_str(), is_selected))
            {
                current_item = key;
                changed = true;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }

            // Tooltip
            if (ImGui::IsItemHovered())
            {
                if (ImGui::BeginItemTooltip())
                {
                    Ptr<CTexture> HoveredTex = (CTexture*)iter.second.Get();
                    ImGui::Text(ToString(HoveredTex->GetName()).c_str());
                    ImGui::Text("Texture Width : %d", HoveredTex->GetWidth());
                    ImGui::Text("Texture Height : %d", HoveredTex->GetHeight());
                    ImGui::Image(((CTexture*)HoveredTex.Get())->GetSRV().Get(), ImVec2(100, 100));
                    ImGui::EndTooltip();
                }
            }
        }

        ImGui::EndCombo();
    }

    return changed;
}

bool ImGui_AlignButton(const char* label, float alignment)
{
    alignment = std::clamp(alignment, 0.f, 1.f);

    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}

void ImGui_InputText(const char* label, const string& Text, float alignment)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    if (!Text.empty())
        strcpy_s(buffer, sizeof(buffer), Text.c_str());

    ImGui::InputText(ImGui_LabelPrefix(label, alignment).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
}

bool ImGui_BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = size_arg;
    size.x -= style.FramePadding.x * 2;

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // Render
    const float circleStart = size.x * 0.7f;
    const float circleEnd = size.x;
    const float circleWidth = circleEnd - circleStart;

    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

    const float t = (float)g.Time;
    const float r = size.y / 2;
    const float speed = 1.5f;

    const float a = speed * 0;
    const float b = speed * 0.333f;
    const float c = speed * 0.666f;

    const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
    const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
    const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

    return true;
}

bool ImGui_Spinner(const char* label, float radius, int thickness, const ImU32& color)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start = (int)abs(ImSin((float)g.Time * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + (float)g.Time * 8) * radius, centre.y + ImSin(a + (float)g.Time * 8) * radius));
    }

    window->DrawList->PathStroke(color, false, (float)thickness);

    return true;
}
