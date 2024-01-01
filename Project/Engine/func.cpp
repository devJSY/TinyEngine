#include "pch.h"

#include "CTaskMgr.h"
#include "CRenderMgr.h"

void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
    FTask task = {};
    task.Type = TASK_TYPE::CREATE_OBJECT;
    task.Param_1 = (DWORD_PTR)_LayerIdx;
    task.Param_2 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
    FTask task = {};
    task.Type = TASK_TYPE::DELETE_OBJECT;
    task.Param_1 = (DWORD_PTR)_Target;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::WindowResize(int width, int height)
{
    FTask task;
    task.Type = TASK_TYPE::WINDOW_RESIZE;
    task.Param_1 = (INT_PTR)width;
    task.Param_2 = (INT_PTR)height;

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

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest,
                                   float _Duration)
{
    tDebugShapeInfo info = {};
    info.eShape = DEBUG_SHAPE::RECT;

    info.vWorldPos = _vWorldPos;
    info.vWorldScale = _vWorldScale;
    info.vWorldRot = _vWorldRot;

    info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z) *
                    XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y) *
                    XMMatrixRotationZ(info.vWorldRot.z) *
                    XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

    info.vColor = _Color;
    info.bDepthTest = _bDepthTest;
    info.fDuration = _Duration;

    CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::ScreenShot()
{
    FTask task = {};
    task.Type = TASK_TYPE::SCREENSHOT;
    CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::MouseColorPicking(Vec2 MousePos)
{
    FTask task = {};
    task.Type = TASK_TYPE::MOUSE_COLOR_PICKING;
    task.Param_1 = (INT_PTR)MousePos.x;
    task.Param_2 = (INT_PTR)MousePos.y;
    CTaskMgr::GetInst()->AddTask(task);
}

string WstringTostring(const wstring& wstr)
{
    std::string str(wstr.length(), 0);
    std::transform(wstr.begin(), wstr.end(), str.begin(), [](wchar_t c) { return (char)c; });
    return str;
}

wstring stringToWstring(const string& str)
{
    return wstring().assign(str.begin(), str.end());
}

std::string currentDateTime()
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
