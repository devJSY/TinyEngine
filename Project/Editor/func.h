#pragma once
void ImGui_SetWindowClass(EDITOR_TYPE _Type);

// =====================================
// ImGui Utility
// =====================================
bool ImGui_DrawVec3Control(const string& label, Vec3& values, float speed = 0.1f, float min = 0.f, float max = 0.f, float resetValue = 0.0f,
                           float columnWidth = 100.0f);
string ImGui_LabelPrefix(const char* const label, float alignment = 0.5f);
bool ImGui_ComboUI(const string& caption, string& current_item, const std::vector<string>& items);
template <typename T>
bool ImGui_ComboUI(const string& caption, string& current_item, const std::map<std::wstring, T>& items);
bool ImGui_TexturesComboUI(const string& caption, string& current_item);
bool ImGui_AlignButton(const char* label, float alignment = 0.5f);
void ImGui_InputText(const char* label, const string& Text, float alignment = 0.5f);

bool ImGui_BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
bool ImGui_Spinner(const char* label, float radius, int thickness, const ImU32& color);

template <typename T>
inline bool ImGui_ComboUI(const string& caption, string& current_item, const std::map<std::wstring, T>& items)
{
    static ImGuiTextFilter filter;
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

        for (const auto& iter : items)
        {
            std::string strKey = ToString(iter.first);
            bool is_selected = (current_item == strKey);

            if (!filter.PassFilter(strKey.c_str()))
                continue;

            if (ImGui::Selectable(strKey.c_str(), is_selected))
            {
                current_item = strKey;
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
