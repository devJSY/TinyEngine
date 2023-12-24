#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
    : m_hRenderWnd(nullptr)
    , m_arrCB{}
    , m_arrRS{}
    , m_arrDS{}
    , m_arrBS{}
{
}

CDevice::~CDevice()
{
    Delete_Array(m_arrCB);
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
    // 출력 윈도우
    m_hRenderWnd = _hWnd;

    m_vRenderResolution = _vResolution;

    // 장치 초기화
    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    const D3D_FEATURE_LEVEL featureLevels[2] = {D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
                                                D3D_FEATURE_LEVEL_9_3};
    D3D_FEATURE_LEVEL featureLevel;
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels,
                                 ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, m_Device.GetAddressOf(), &featureLevel,
                                 m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // 스왚체인 생성
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateBufferAndView()))
    {
        MessageBox(nullptr, L"타겟 및 View 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"Rasterizer State 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"DepthStencil State 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"Blend State 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateViewport()))
    {
        MessageBox(nullptr, L"Viewport 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateConstBuffer()))
    {
        MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void CDevice::ClearRenderTarget(const Vec4& Color)
{
    m_Context->ClearRenderTargetView(m_RTView.Get(), Color);
    m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

    m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
    m_SwapChain->Present(0, 0);
}

void CDevice::Resize(Vec2 resolution)
{
    m_vRenderResolution = resolution;

    CreateBufferAndView();
    CreateViewport();
}

int CDevice::CreateViewport()
{
    D3D11_VIEWPORT ViewportDesc = {};

    ViewportDesc.MinDepth = 0;
    ViewportDesc.MaxDepth = 1.f;

    ViewportDesc.TopLeftX = 0;
    ViewportDesc.TopLeftY = 0;
    ViewportDesc.Width = m_vRenderResolution.x;
    ViewportDesc.Height = m_vRenderResolution.y;

    CONTEXT->RSSetViewports(1, &ViewportDesc);

    return S_OK;
}

int CDevice::CreateSampler()
{
    D3D11_SAMPLER_DESC tSamDesc = {};
    ZeroMemory(&tSamDesc, sizeof(tSamDesc));

    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[(UINT)SS_TYPE::LINEAR].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[(UINT)SS_TYPE::POINT].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::LINEAR, 1, m_Sampler[(UINT)SS_TYPE::LINEAR].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::LINEAR, 1, m_Sampler[(UINT)SS_TYPE::LINEAR].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::LINEAR, 1, m_Sampler[(UINT)SS_TYPE::LINEAR].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::LINEAR, 1, m_Sampler[(UINT)SS_TYPE::LINEAR].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::LINEAR, 1, m_Sampler[(UINT)SS_TYPE::LINEAR].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::POINT, 1, m_Sampler[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::POINT, 1, m_Sampler[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::POINT, 1, m_Sampler[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::POINT, 1, m_Sampler[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::POINT, 1, m_Sampler[(UINT)SS_TYPE::POINT].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_Sampler[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_Sampler[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_Sampler[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_Sampler[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_Sampler[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());

    return S_OK;
}

int CDevice::CreateSwapChain()
{
    // SwapChain 생성 구조체
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    // SwapChain 이 관리하는 Buffer(RenderTarget)의 구성 정보
    tDesc.BufferCount = 1;
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tDesc.BufferDesc.RefreshRate.Denominator = 1;
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    tDesc.Flags = 0;

    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;

    tDesc.Windowed = true;             // 창모드
    tDesc.OutputWindow = m_hRenderWnd; // SwapChain 의 출력 윈도우 지정

    // 스왚체인 생성기능을 가지고 있는 Factory 에 접근한다.
    ComPtr<IDXGIDevice> pIdxgiDevice = nullptr;
    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    ComPtr<IDXGIFactory> pFactory = nullptr;

    m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
    pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    // SwapChain 생성
    if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateBufferAndView()
{
    // ============
    // BackBuffer
    // ============
    m_RTTex.Reset();
    m_RTView.Reset();
    m_SwapChain->ResizeBuffers(0, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y, DXGI_FORMAT_UNKNOWN, 0);

    // 렌더타겟 텍스쳐를 스왚체인으로부터 얻어온다.
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

    // RenderTargetView
    m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());

    // =========================
    // DepthStencillTexture 생성
    // =========================
    m_DSTex.Reset();
    m_DSView.Reset();
    D3D11_TEXTURE2D_DESC Desc = {};

    // 픽셀 포맷은 Depth 3바이트 Stencil 1바이트
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // DepthStencilState 텍스쳐 해상도는 반드시 RenderTargetTexture 와 동일해야한다.
    Desc.Width = (UINT)m_vRenderResolution.x;
    Desc.Height = (UINT)m_vRenderResolution.y;

    // DepthStencil 용도의 텍스쳐
    Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    // CPU 접근 불가
    Desc.CPUAccessFlags = 0;
    Desc.Usage = D3D11_USAGE_DEFAULT;

    // 샘플링
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    // 저퀄리티 버전의 사본 생성여부
    Desc.MipLevels = 1;
    Desc.MiscFlags = 0;

    Desc.ArraySize = 1;

    if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTex.GetAddressOf())))
    {
        return E_FAIL;
    }

    // DepthStencilView
    m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());

    // OM(Output Merge State) 에 RenderTargetTexture 와 DepthStencilTexture 를 전달한다.
    m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

    return S_OK;
}

int CDevice::CreateRasterizerState()
{
    m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    HRESULT result = S_OK;

    D3D11_RASTERIZER_DESC tDesc = {};

    tDesc.CullMode = D3D11_CULL_FRONT;
    tDesc.FillMode = D3D11_FILL_SOLID;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.CullMode = D3D11_CULL_NONE;
    tDesc.FillMode = D3D11_FILL_SOLID;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.CullMode = D3D11_CULL_NONE;
    tDesc.FillMode = D3D11_FILL_WIREFRAME;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    return S_OK;
}

int CDevice::CreateDepthStencilState()
{
    HRESULT hr = S_OK;

    m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

    D3D11_DEPTH_STENCIL_DESC tDesc = {};

    // Less Equal
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;

    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    // Greater
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;
    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GREATER].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    // Greater Equal
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;
    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    // No Test
    tDesc.DepthEnable = false;
    tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;
    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

    // No Write
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_LESS;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    tDesc.StencilEnable = false;
    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    // NoTest NoWrite
    tDesc.DepthEnable = false;
    tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    tDesc.StencilEnable = false;
    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    return S_OK;
}

int CDevice::CreateBlendState()
{
    m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

    D3D11_BLEND_DESC tDesc = {};

    // AlphaBlend
    tDesc.AlphaToCoverageEnable = false;
    tDesc.IndependentBlendEnable = false;

    tDesc.RenderTarget[0].BlendEnable = true;
    tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

    tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

    tDesc.AlphaToCoverageEnable = false;
    tDesc.IndependentBlendEnable = false;

    tDesc.RenderTarget[0].BlendEnable = true;
    tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

    tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

    return S_OK;
}

int CDevice::CreateConstBuffer()
{
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
    m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

    m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST] = new CConstBuffer;
    m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST]->Create(sizeof(tMaterialData), 1);

    m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA] = new CConstBuffer;
    m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA]->Create(sizeof(tGlobal), 1);

    return S_OK;
}
