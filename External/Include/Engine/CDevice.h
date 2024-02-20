#pragma once

class CConstBuffer;

// DirectX11 기준으로 GPU 제어
class CDevice : public CSingleton<CDevice>
{
    SINGLE(CDevice);

private:
    HWND m_hRenderWnd;
    Vec2 m_vRenderResolution;

    ComPtr<ID3D11Device> m_Device;         // GPU 메모리 관리, 객체 생성
    ComPtr<ID3D11DeviceContext> m_Context; // GPU 렌더링 명령

    ComPtr<IDXGISwapChain> m_SwapChain; // 스왚체인(출력 버퍼 지정)

    Ptr<CTexture> m_RenderTargetTex;
    Ptr<CTexture> m_FloatRTTex;
    Ptr<CTexture> m_DSTex;

    ComPtr<ID3D11RasterizerState> m_arrRS[(UINT)RS_TYPE::END];   // 레스터라이저
    ComPtr<ID3D11DepthStencilState> m_arrDS[(UINT)DS_TYPE::END]; // 깊이스탠실
    ComPtr<ID3D11BlendState> m_arrBS[(UINT)BS_TYPE::END];        // 블랜드

    ComPtr<ID3D11SamplerState> m_arrSS[(UINT)SS_TYPE::END]; // Sampler

    CConstBuffer* m_arrCB[(UINT)CB_TYPE::END]; // Constant Buffer

    D3D11_VIEWPORT m_Viewport;

public:
    int init(HWND _hWnd, Vec2 _vResolution);
    void Clear_Buffers(const Vec4& Color);
    void Present();

    ID3D11Device* GetDevice() const { return m_Device.Get(); }
    ID3D11DeviceContext* GetContext() const { return m_Context.Get(); }

    CConstBuffer* GetConstBuffer(CB_TYPE _type) const { return m_arrCB[(UINT)_type]; }
    Vec2 GetRenderResolution() const { return m_vRenderResolution; }

    ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) const { return m_arrRS[(UINT)_Type]; }
    ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) const { return m_arrDS[(UINT)_Type]; }
    ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) const { return m_arrBS[(UINT)_Type]; }

    Ptr<CTexture> GetRenderTargetTex() const { return m_RenderTargetTex; }

public:
    void Resize(Vec2 resolution);
    void SetRenderTarget();
    void SetFloatRenderTarget();
    void SetViewport(float _Width = 0, float _Height = 0);
    void ClearDepth(FLOAT _depth = 1.f);
    void ClearStencil(UINT8 _stencil = 0);

private:
    int CreateSwapChain();
    int CreateBuffer();
    int CreateRasterizerState();
    int CreateDepthStencilState();
    int CreateBlendState();
    int CreateConstBuffer();
    int CreateViewport();
    int CreateSamplerState();
};
