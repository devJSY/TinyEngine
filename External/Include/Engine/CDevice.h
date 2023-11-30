#pragma once

// DirectX11 기준으로 GPU 제어
class CDevice : public CSingleton<CDevice>
{
    SINGLE(CDevice);

  private:
    ComPtr<ID3D11Device>            m_Device;    // GPU 메모리 관리, 객체 생성
    ComPtr<ID3D11DeviceContext>     m_Context;   // GPU 렌더링 명령

    ComPtr<IDXGISwapChain>          m_SwapChain; // 스왚체인(출력 버퍼 지정)

    // OM(OutputMergeState)
    ComPtr<ID3D11Texture2D>         m_RTTex;     // 렌더타겟 텍스쳐
    ComPtr<ID3D11RenderTargetView>  m_RTView;    // 렌더타겟 뷰
                                                 
    ComPtr<ID3D11Texture2D>         m_DSTex;     // 뎊스 스텐실 텍스쳐
    ComPtr<ID3D11DepthStencilView>  m_DSView;    // 뎊스 스텐실 뷰

    HWND                            m_hRenderWnd;   
    Vec2                            m_vRenderResolution;

  public:
    int init(HWND _hWnd, Vec2 _vResolution);

  private:
    int CreateSwapChain();
    int CreateTargetView();
};
