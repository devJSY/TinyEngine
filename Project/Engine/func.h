#pragma once

class CGameObject;
class CLevel;
class CScript;
class CPrefab;
template <typename T>
class Ptr;
class CAsset;

namespace GamePlayStatic
{
    void SpawnGameObject(CGameObject* _Target, int _LayerIdx);
    void AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject);
    void DestroyGameObject(CGameObject* _Target);
    void WindowResize(int width, int height);
    void CloneGameObject(CGameObject* _OriginObj);

    void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugBox(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugBox(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugSphere(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void ScreenShot();
    void MouseColorPicking(Vec2 MousePos);
    void MouseRayPicking(Vec2 MousePos);

    void AddComponent(CGameObject* _pObj, COMPONENT_TYPE _type);
    void RemoveComponent(CGameObject* _pObj, COMPONENT_TYPE _type);
    void RemoveScript(CGameObject* _pObj, CScript* _script);
    void LayerChange(CGameObject* _pObj, int LayerIdx);

    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextState);
    void ChangeLevelState(CLevel* _NextLevel, LEVEL_STATE _NextState);

    void DeleteAsset(ASSET_TYPE _type, Ptr<CAsset> _Asset);

    void Physics2D_Event(CGameObject* _pObj, Physics2D_EVENT_TYPE _Type);

    void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
    void Play2DBGM(const wstring& _SoundPath, float _Volume);
}

string ToString(const wstring& wstr);
wstring ToWstring(const string& str);
string currentDateTime();
Vec4 HashIDToColor(int hash);
void ReadImage(const std::string filename, std::vector<uint8_t>& image, int& width, int& height);
ComPtr<ID3D11Texture2D> CreateStagingTexture(const int width, const int height, const std::vector<uint8_t>& image,
                                             const DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM, const int mipLevels = 1,
                                             const int arraySize = 1);

// =====================================
// Save / Load
// =====================================
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

#include "CAssetMgr.h"

template <typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
    bool bAssetExist = false;
    _Asset == nullptr ? bAssetExist = false : bAssetExist = true;

    fwrite(&bAssetExist, sizeof(bool), 1, _File);

    if (bAssetExist)
    {
        SaveWString(_Asset->GetKey(), _File);
        SaveWString(_Asset->GetRelativePath(), _File);
    }
}

template <typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
    bool bAssetExist = false;
    fread(&bAssetExist, sizeof(bool), 1, _File);

    if (bAssetExist)
    {
        wstring strKey, strRelativePath;
        LoadWString(strKey, _File);
        LoadWString(strRelativePath, _File);

        _Asset = CAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
    }
}

wstring OpenFileDialog(const wstring& strRelativePath, const wchar_t* filter = L"All\0*.*\0"); // 전체 경로 반환
wstring SaveFileDialog(const wstring& strRelativePath, const wchar_t* filter = L"All\0*.*\0"); // 전체 경로 반환

// =====================================
// ImGui Utility
// =====================================
void ImGui_DrawVec3Control(const string& label, Vec3& values, float speed = 0.1f, float min = 0.f, float max = 0.f, float resetValue = 0.0f,
                           float columnWidth = 100.0f);
string ImGui_LabelPrefix(const char* const label, float alignment = 0.5f);
bool ImGui_ComboUI(const string& caption, string& current_item, const std::vector<string>& items);
bool ImGui_TexturesComboUI(const string& caption, string& current_item);
template <typename T>
bool ImGui_ComboUI(const string& caption, string& current_item, const std::map<std::wstring, T>& items);
bool ImGui_AlignButton(const char* label, float alignment = 0.5f);
void ImGui_InputText(const char* label, const string& Text, float alignment = 0.5f);

void ImGui_SetWindowClass_LevelEditor();
void ImGui_SetWindowClass_MaterialEditor();
void ImGui_SetWindowClass_Blueprint();
void ImGui_SetWindowClass_SpriteEditor();
void ImGui_SetWindowClass_TileMapEditor();

// =====================================
// template
// =====================================
template <typename T, UINT SIZE>
void Delete_Array(T* (&Arr)[SIZE])
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        if (nullptr != Arr[i])
        {
            delete Arr[i];
            Arr[i] = nullptr;
        }
    }
}

template <typename T>
void Delete_Vec(vector<T>& _vec)
{
    for (size_t i = 0; i < _vec.size(); ++i)
    {
        if (nullptr != _vec[i])
        {
            delete _vec[i];
            _vec[i] = nullptr;
        }
    }

    _vec.clear();
}

template <typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
    for (const auto& pair : _map)
    {
        if (nullptr != pair.second)
            delete pair.second;
    }
    _map.clear();
}

template <typename T>
inline bool ImGui_ComboUI(const string& caption, string& current_item, const std::map<std::wstring, T>& items)
{
    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
    {
        for (const auto& iter : items)
        {
            std::string strKey = ToString(iter.first);
            bool is_selected = (current_item == strKey);
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
