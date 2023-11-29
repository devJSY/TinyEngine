#pragma once

// DirectX11 기준으로 GPU 제어
class CDevice : public CSingleton<CDevice>
{
    SINGLE(CDevice);

  public:
    int init(HWND _hWnd, Vec2 _vResolution);

  private:
    ID3D11Device* m_Device;         // GPU 메모리 관리 및 객체 생성
    ID3D11DeviceContext* m_Context; // GPU 렌더링 명령
};
