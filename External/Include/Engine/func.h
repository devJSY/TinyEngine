#pragma once

class CGameObject;

namespace GamePlayStatic
{
    void SpawnGameObject(CGameObject* _Target, int _LayerIdx);
    void DestroyGameObject(CGameObject* _Target);
    void WindowResize(int width, int height);

    void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
    void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest,
                       float _Duration = 0.f);

    //void DrawDebugCircle();
    //void DrawDebugCube();
    //void DrawDebugSphere();

    void ScreenShot();
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

string WstringTostring(const wstring& wstr);
wstring stringToWstring(const string& str);
std::string currentDateTime();
Vec4 HashIDToColor(int hash);

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
