#include "pch.h"

#include "CDevice.h"

#include "CTaskMgr.h"
#include "CRenderMgr.h"
#include "CEditorMgr.h"
#include "CEngine.h"
#include "func.h"
#include "components.h"

// stb
extern "C"
{
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
}

void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
    tTask task = {};
    task.Type = TASK_TYPE::CREATE_OBJECT;
    task.Param_1 = (DWORD_PTR)_LayerIdx;
    task.Param_2 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddChildObject(CGameObject* _ParentObject, CGameObject* _ChildObject)
{
    tTask task = {};
    task.Type = TASK_TYPE::ADD_CHILD;
    task.Param_1 = (DWORD_PTR)_ParentObject;
    task.Param_2 = (DWORD_PTR)_ChildObject;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
    tTask task = {};
    task.Type = TASK_TYPE::DELETE_OBJECT;
    task.Param_1 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::WindowResize(int width, int height)
{
    tTask task;
    task.Type = TASK_TYPE::WINDOW_RESIZE;
    task.Param_1 = (INT_PTR)width;
    task.Param_2 = (INT_PTR)height;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::CloneGameObject(CGameObject* _OriginObj)
{
    tTask task;
    task.Type = TASK_TYPE::CLONE_OBJECT;
    task.Param_1 = (DWORD_PTR)_OriginObj;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::RECT;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::RECT;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = _vWorldScale;
    info.vWorldRot = _vWorldRot;

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CIRCLE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CIRCLE;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
    info.vWorldRot = Vec3(0.f, 0.f, 0.f);

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::CROSS;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fScale, _fScale, _fScale);
    info.vWorldRot = Vec3(0.f, 0.f, 0.f);

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugBox(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::BOX;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugBox(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::BOX;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = _vWorldScale;
    info.vWorldRot = _vWorldRot;
    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugSphere(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::SPHERE;
    info.matWorld = _WorldMat;
    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::SPHERE;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = Vec3(_fRadius, _fRadius, _fRadius);
    info.vWorldRot = Vec3(0.f, 0.f, 0.f);

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) * XMMatrixRotationX(info.vWorldRot.x) *
                    XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::ScreenShot()
{
    tTask task = {};
    task.Type = TASK_TYPE::SCREENSHOT;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::MouseColorPicking(Vec2 MousePos)
{
    tTask task = {};
    task.Type = TASK_TYPE::MOUSE_COLOR_PICKING;
    task.Param_1 = (INT_PTR)MousePos.x;
    task.Param_2 = (INT_PTR)MousePos.y;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::MouseRayPicking(Vec2 MousePos)
{
    tTask task = {};
    task.Type = TASK_TYPE::MOUSE_RAY_PICKING;
    task.Param_1 = (INT_PTR)MousePos.x;
    task.Param_2 = (INT_PTR)MousePos.y;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::MouseCollision2DPicking(Vec2 MousePos)
{
    tTask task = {};
    task.Type = TASK_TYPE::MOUSE_COLLISION2D_PICKING;
    task.Param_1 = (INT_PTR)MousePos.x;
    task.Param_2 = (INT_PTR)MousePos.y;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddComponent(CGameObject* _pObj, COMPONENT_TYPE _type)
{
    tTask task = {};
    task.Type = TASK_TYPE::ADD_COMPONENT;
    task.Param_1 = (INT_PTR)_pObj;
    task.Param_2 = (INT_PTR)_type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::RemoveComponent(CGameObject* _pObj, COMPONENT_TYPE _type)
{
    tTask task = {};
    task.Type = TASK_TYPE::REMOVE_COMPONENT;
    task.Param_1 = (INT_PTR)_pObj;
    task.Param_2 = (INT_PTR)_type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::RemoveScript(CGameObject* _pObj, CScript* _script)
{
    tTask task = {};
    task.Type = TASK_TYPE::REMOVE_SCRIPT;
    task.Param_1 = (INT_PTR)_pObj;
    task.Param_2 = (INT_PTR)_script;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::LayerChange(CGameObject* _pObj, int LayerIdx)
{
    tTask task = {};
    task.Type = TASK_TYPE::CHANGE_LAYER;
    task.Param_1 = (DWORD_PTR)_pObj;
    task.Param_2 = (INT_PTR)LayerIdx;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextState)
{
    tTask task = {};
    task.Type = TASK_TYPE::CHANGE_LEVEL;
    task.Param_1 = (DWORD_PTR)_NextLevel;
    task.Param_2 = (DWORD_PTR)_NextState;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::ChangeLevelState(CLevel* _NextLevel, LEVEL_STATE _NextState)
{
    tTask task = {};
    task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
    task.Param_1 = (DWORD_PTR)_NextLevel;
    task.Param_2 = (DWORD_PTR)_NextState;

    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DeleteAsset(ASSET_TYPE _type, Ptr<CAsset> _Asset)
{
    tTask task = {};
    task.Type = TASK_TYPE::DELETE_ASSET;
    task.Param_1 = (DWORD_PTR)_type;
    task.Param_2 = (DWORD_PTR)_Asset.Get();
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::Physics2D_Event(CGameObject* _pObj, Physics2D_EVENT_TYPE _Type)
{
    tTask task = {};
    task.Type = TASK_TYPE::PHYSICS2D_EVNET;
    task.Param_1 = (DWORD_PTR)_pObj;
    task.Param_2 = (DWORD_PTR)_Type;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    if (nullptr != pSound)
    {
        pSound->Play(_Loop, _Volume, _Overlap);
    }
}

void GamePlayStatic::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
    static Ptr<CSound> CurBGM = nullptr;

    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    if (nullptr != pSound)
    {
        if (nullptr != CurBGM)
        {
            CurBGM->Stop();
        }

        pSound->Play(0, _Volume);
        CurBGM = pSound;
    }
}

string ToString(const wstring& wstr)
{
    string str(wstr.length(), 0);
    std::transform(wstr.begin(), wstr.end(), str.begin(), [](wchar_t c) { return (char)c; });
    return str;
}

wstring ToWstring(const string& str)
{
    return wstring().assign(str.begin(), str.end());
}

string currentDateTime()
{
    // 현재 시간을 구하기
    std::time_t currentTime = std::time(nullptr);

    // 지역 시간 구조체
    std::tm localTime;

    // localtime_s 함수를 사용하여 안전하게 지역 시간 얻기
    localtime_s(&localTime, &currentTime);

    // 포맷 문자열
    const char* format = "%Y-%m-%d_%H-%M-%S";

    // 포맷에 맞게 문자열로 변환
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), format, &localTime);

    return buffer;
}

Vec4 HashIDToColor(int hash)
{
    int r = (hash >> 16) & 0xff;
    int g = (hash >> 8) & 0xff;
    int b = hash & 0xff;

    return Vec4(static_cast<float>(r / 255.f), static_cast<float>(g / 255.f), static_cast<float>(b / 255.f), 1.f);
}

void ReadImage(const std::string filename, std::vector<uint8_t>& image, int& width, int& height)
{
    int channels;

    unsigned char* img = stbi_load((ToString(CPathMgr::GetContentPath()) + filename).c_str(), &width, &height, &channels, 0);

    // 4채널로 만들어서 복사
    image.resize(width * height * 4);

    if (channels == 1)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            uint8_t g = img[i * channels + 0];
            for (size_t c = 0; c < 4; c++)
            {
                image[4 * i + c] = g;
            }
        }
    }
    else if (channels == 2)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            for (size_t c = 0; c < 2; c++)
            {
                image[4 * i + c] = img[i * channels + c];
            }
            image[4 * i + 2] = 255;
            image[4 * i + 3] = 255;
        }
    }
    else if (channels == 3)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                image[4 * i + c] = img[i * channels + c];
            }
            image[4 * i + 3] = 255;
        }
    }
    else if (channels == 4)
    {
        for (size_t i = 0; i < width * height; i++)
        {
            for (size_t c = 0; c < 4; c++)
            {
                image[4 * i + c] = img[i * channels + c];
            }
        }
    }
    else
    {
        LOG(Error, "Cannot read %d channels", channels);
    }

    delete[] img;
}

ComPtr<ID3D11Texture2D> CreateStagingTexture(const int width, const int height, const std::vector<uint8_t>& image, const DXGI_FORMAT pixelFormat,
                                             const int mipLevels, const int arraySize)
{
    // 스테이징 텍스춰 만들기
    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = mipLevels;
    txtDesc.ArraySize = arraySize;
    txtDesc.Format = pixelFormat;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_STAGING;
    txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

    ComPtr<ID3D11Texture2D> stagingTexture;
    if (FAILED(DEVICE->CreateTexture2D(&txtDesc, NULL, stagingTexture.GetAddressOf())))
    {
        LOG(Error, "Create Failed Staging Texture");
    }

    // CPU에서 이미지 데이터 복사
    size_t pixelSize = sizeof(uint8_t) * 4;
    if (pixelFormat == DXGI_FORMAT_R16G16B16A16_FLOAT)
    {
        pixelSize = sizeof(uint16_t) * 4;
    }

    D3D11_MAPPED_SUBRESOURCE ms;
    CONTEXT->Map(stagingTexture.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);
    uint8_t* pData = (uint8_t*)ms.pData;
    for (UINT h = 0; h < UINT(height); h++)
    { // 가로줄 한 줄씩 복사
        memcpy(&pData[h * ms.RowPitch], &image[h * width * pixelSize], width * pixelSize);
    }
    CONTEXT->Unmap(stagingTexture.Get(), NULL);

    return stagingTexture;
}

void SaveWString(const wstring& _str, FILE* _File)
{
    UINT iLen = (UINT)_str.length();
    fwrite(&iLen, sizeof(UINT), 1, _File);
    fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
    wchar_t szBuffer[256] = {};

    UINT iLen = 0;
    fread(&iLen, sizeof(UINT), 1, _File);
    fread(szBuffer, sizeof(wchar_t), iLen, _File);

    _str = szBuffer;
}

void SaveAssetRef(Ptr<CAsset> _Asset, FILE* _File)
{
    int i = 0;
    if (nullptr == _Asset)
    {
        fwrite(&i, sizeof(i), 1, _File);
    }
    else
    {
        i = 1;
        fwrite(&i, sizeof(i), 1, _File);
        SaveWString(_Asset->GetKey(), _File);
        SaveWString(_Asset->GetRelativePath(), _File);
    }
}

wstring OpenFileDialog(const wstring& strRelativePath, const wchar_t* filter)
{
    wchar_t szName[256] = {};
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CEngine::GetInst()->GetMainWind();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring Path = CPathMgr::GetContentPath();
    Path += strRelativePath;
    ofn.lpstrInitialDir = Path.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn) == TRUE)
        return ofn.lpstrFile;

    return wstring();
}

wstring SaveFileDialog(const wstring& strRelativePath, const wchar_t* filter)
{
    wchar_t szName[256] = {};
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CEngine::GetInst()->GetMainWind();
    ofn.lpstrFile = szName;
    ofn.nMaxFile = sizeof(szName);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;

    wstring Path = CPathMgr::GetContentPath();
    Path += strRelativePath;
    ofn.lpstrInitialDir = Path.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileName(&ofn) == TRUE)
        return ofn.lpstrFile;

    return wstring();
}

void ImGui_DrawVec3Control(const string& label, Vec3& values, float speed, float min, float max, float resetValue, float columnWidth)
{
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
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, speed, min, max, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
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

void ImGui_SetWindowClass(EDITOR_TYPE _Type)
{
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetID(ToString(CEditorMgr::GetInst()->GetEditor(_Type)->GetName()).c_str());
    window_class.DockNodeFlagsOverrideSet = 0;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);
}
