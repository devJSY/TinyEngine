#pragma once

class CGameObject;
class CLevel;

namespace GamePlayStatic
{
    void SpawnGameObject(CGameObject* _Target, int _LayerIdx);
    void DestroyGameObject(CGameObject* _Target);
    void WindowResize(int width, int height);

    void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest,
                       float _Duration = 0.f);

    void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    // void DrawDebugCube();
    // void DrawDebugSphere();

    void ScreenShot();
    void MouseColorPicking(Vec2 MousePos);
    void MouseRayPicking(Vec2 MousePos);

    void AddComponent(CGameObject* _pObj, COMPONENT_TYPE _type);
    void RemoveComponent(CGameObject* _pObj, COMPONENT_TYPE _type);
    void LayerChange(CGameObject* _pObj, int LayerIdx);

    void LevelChange(CLevel* _Level);
}

string ToString(const wstring& wstr);
wstring ToWstring(const string& str);
string currentDateTime();
Vec4 HashIDToColor(int hash);
string GetComponentName(COMPONENT_TYPE type);
string GetAssetTypeName(ASSET_TYPE type);

// =====================================
// Save / Load
// =====================================
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

class CAsset;
template <typename T>
class Ptr;

void SaveAssetRef(Ptr<CAsset> _Asset, FILE* _File);

#include "CAssetMgr.h"
template <typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
    int i = 0;
    fread(&i, sizeof(i), 1, _File);

    if (i)
    {
        wstring strKey, strRelativePath;
        LoadWString(strKey, _File);
        LoadWString(strRelativePath, _File);

        _Asset = CAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
    }
}

wstring OpenFile(const wstring& strRelativePath, const wchar_t* filter = L"All\0*.*\0"); // 전체 경로 반환
wstring SaveFile(const wstring& strRelativePath, const wchar_t* filter = L"All\0*.*\0"); // 전체 경로 반환

// =====================================
// ImGui Utility
// =====================================
void ImGuiDrawVec3Control(const string& label, Vec3& values, float speed = 0.1f, float min = 0.f, float max = 0.f,
                     float resetValue = 0.0f, float columnWidth = 100.0f);
string ImGuiLabelPrefix(const char* const label, float alignment = 0.5f);
bool ImGuiComboUI(const string& caption, string& current_item, const std::vector<string>& items);
bool ImGuiAlignButton(const char* label, float alignment = 0.5f);

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

// template <typename T>
// bool IsValid(T*& _Entity)
//{
//     if (nullptr == _Entity)
//     {
//         return false;
//     }
//
//     if (_Entity->IsDead())
//     {
//         _Entity = nullptr;
//         return false;
//     }
//
//     return true;
// }
