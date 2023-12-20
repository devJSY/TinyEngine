#pragma once

class CConstBuffer;

// DirectX11 기준으로 GPU 제어
class CDevice : public CSingleton<CDevice>
{
    SINGLE(CDevice);

private:
    ComPtr<ID3D11Device> m_Device;         // GPU 메모리 관리, 객체 생성
    ComPtr<ID3D11DeviceContext> m_Context; // GPU 렌더링 명령

    ComPtr<IDXGISwapChain> m_SwapChain; // 스왚체인(출력 버퍼 지정)

    // OM(OutputMergeState)
    ComPtr<ID3D11Texture2D> m_RTTex;         // 렌더타겟 텍스쳐
    ComPtr<ID3D11RenderTargetView> m_RTView; // 렌더타겟 뷰

    ComPtr<ID3D11Texture2D> m_DSTex;         // 뎊스 스텐실 텍스쳐
    ComPtr<ID3D11DepthStencilView> m_DSView; // 뎊스 스텐실 뷰

    HWND m_hRenderWnd;
    Vec2 m_vRenderResolution;

    CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];

private:
    // ImGui Viewport
    ComPtr<ID3D11Texture2D> m_ViewportRTTex;
    ComPtr<ID3D11ShaderResourceView> m_ViewportSRView;

public:
    int init(HWND _hWnd, Vec2 _vResolution);
    void ClearRenderTarget(float (&Color)[4]);
    void Present();

    ID3D11Device* GetDevice() const { return m_Device.Get(); }
    ID3D11DeviceContext* GetContext() const { return m_Context.Get(); }

    CConstBuffer* GetConstBuffer(CB_TYPE _type) const { return m_arrCB[(UINT)_type]; }
    Vec2 GetRenderResolution() { return m_vRenderResolution; }

public:
    ID3D11ShaderResourceView* GetViewportSRV() const { return m_ViewportSRView.Get(); }

public:
    void CopyToViewport();
    int ReSize(Vec2 resolution);

private:
    int CreateBuffers();
    void SetViewport();

private:
    int CreateSwapChain();
    int CreateTargetView();
    int CreateConstBuffer();
};
