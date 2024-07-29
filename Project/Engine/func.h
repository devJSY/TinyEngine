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
    void DetachObject(CGameObject* _Target);
    void AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject);
    void AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject, tBoneSocket* _BoneSocket);
    void AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject, const std::wstring& _BoneSocketName);
    void DestroyGameObject(CGameObject* _Target);
    void WindowResize(int width, int height);
    void CloneGameObject(CGameObject* _OriginObj, CGameObject* _OriginParentObj);

    void DrawDebugLine(const Matrix& _WorldMat, Vec3 _p1, Vec3 _p2, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugLine(Vec3 _vWorldPos, Vec3 _vDir, float _fLength, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugBox(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugBox(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugSphere(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCone(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void DrawDebugCapsule(const Matrix& _WorldMat, float _fRadius, float _HalfHeight, AXIS_TYPE _Axis, Vec3 _Color, bool _bDepthTest,
                          float _Duration = 0.f);

    void DrawDebugPolygon(const Matrix& _WorldMat, Vec3 _Color, const vector<Vec2>& _Polygon, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugPolygon(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, const vector<Vec2>& _Polygon, bool _bDepthTest,
                          float _Duration = 0.f);

    void DrawDebugFrustum(const Vec3 WorldVertex[8], Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugMesh(const Matrix& _WorldMat, Ptr<CMesh> _pMesh, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

    void ScreenShot();
    void MouseColorPicking(Vec2 MousePos);
    void MouseCollision2DPicking(Vec2 MousePos);

    void AddComponent(CGameObject* _pObj, COMPONENT_TYPE _type);
    void RemoveComponent(CGameObject* _pObj, COMPONENT_TYPE _type);
    void RemoveScript(CGameObject* _pObj, CScript* _script);
    void LayerChange(CGameObject* _pObj, int LayerIdx);

    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextState);
    void ChangeLevelState(CLevel* _NextLevel, LEVEL_STATE _NextState);

    void DeleteAsset(ASSET_TYPE _type, Ptr<CAsset> _Asset);

    void Physics2D_Event(CGameObject* _pObj, Physics2D_EVENT_TYPE _Type);
    void Physics_Event(CGameObject* _pObj, Physics_EVENT_TYPE _Type);

    void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
    void Play2DBGM(const wstring& _SoundPath, float _Volume);
}

int GetRandomInt(int _Min, int _Max);
float GetRandomfloat(float _Min, float _Max);
float Lerp(float A, float B, float Alpha);

string ToString(const wstring& wstr);
wstring ToWstring(const string& str);
string currentDateTime();
Vec4 HashIDToColor(int hash);
void ReadImage(const std::string filename, std::vector<uint8_t>& image, int& width, int& height);
ComPtr<ID3D11Texture2D> CreateStagingTexture(const int width, const int height, const std::vector<uint8_t>& image,
                                             const DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM, const int mipLevels = 1,
                                             const int arraySize = 1);

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);
FbxAMatrix GetFbxMatrixFromMatrix(Matrix& _mat);

// DXGI_FORMAT -> Size(Byte)
int GetSizeofFormat(DXGI_FORMAT _eFormat);

// =====================================
// Save / Load
// =====================================
UINT SaveWStringToFile(const wstring& _str, FILE* _File);
UINT LoadWStringFromFile(wstring& _str, FILE* _File);

#include "CAssetMgr.h"

template <typename T>
UINT SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
    UINT MemoryByte = 0;

    bool bAssetExist = false;
    _Asset == nullptr ? bAssetExist = false : bAssetExist = true;

    fwrite(&bAssetExist, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    if (bAssetExist)
    {
        MemoryByte += SaveWStringToFile(_Asset->GetKey(), _File);
        MemoryByte += SaveWStringToFile(_Asset->GetRelativePath(), _File);
    }

    return MemoryByte;
}

template <typename T>
UINT LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
    UINT MemoryByte = 0;

    bool bAssetExist = false;
    fread(&bAssetExist, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    if (bAssetExist)
    {
        wstring strKey, strRelativePath;
        MemoryByte += LoadWStringFromFile(strKey, _File);
        MemoryByte += LoadWStringFromFile(strRelativePath, _File);

        _Asset = CAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
    }

    return MemoryByte;
}

wstring OpenFileDialog(const wstring& strRelativePath, const wchar_t* filter = L"All\0*.*\0"); // 전체 경로 반환
wstring SaveFileDialog(const wstring& strRelativePath, const wchar_t* filter = L"All\0*.*\0"); // 전체 경로 반환
void OpenFileDialog(vector<wstring>& _FilesName, const wstring& _RelativePath = L"",
                    const vector<std::pair<wstring, wstring>>& filter = {
                        {L"All Files", L"*.*"}, {L"Text Files", L"*.txt"}, {L"FBX Files", L"*.fbx"}});
Vec2 LoadMeta(const wstring& _strMetaRelativePath);

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

void ImGui_SetWindowClass(EDITOR_TYPE _Type);

// =====================================
// template
// =====================================
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