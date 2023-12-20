#pragma once

class CEngine : public CSingleton<CEngine>
{
    SINGLE(CEngine);

private:
    HWND m_hMainWnd;    // 메인 윈도우 핸들
    Vec2 m_vResolution; // 해상도 정보

public:
    void SetResolution(Vec2 resolution) { m_vResolution = resolution; }
    HWND GetMainWind() const { return m_hMainWnd; }

public:
    int init(HWND _hWnd, Vec2 _vResolution);
    void progress();

};
