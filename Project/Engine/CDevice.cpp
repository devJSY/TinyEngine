#include "pch.h"
#include "CDevice.h"

#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CConstBuffer.h"

CDevice::CDevice()
    : m_hRenderWnd(nullptr)
    , m_arrRS{}
    , m_arrDS{}
    , m_arrBS{}
    , m_arrSS{}
    , m_arrCB{}
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
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels),
                                 D3D11_SDK_VERSION, m_Device.GetAddressOf(), &featureLevel, m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateBuffer()))
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

    if (FAILED(CreateConstBuffer()))
    {
        MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSamplerState()))
    {
        MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Ignore D3D11 Warning
    ComPtr<ID3D11Debug> d3dDebug;
    HRESULT hr = m_Device.As(&d3dDebug);
    if (SUCCEEDED(hr))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        hr = d3dDebug.As(&d3dInfoQueue);
        if (SUCCEEDED(hr))
        {
            D3D11_MESSAGE_ID hide[] = {
                D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET,
            };
            D3D11_INFO_QUEUE_FILTER filter;
            memset(&filter, 0, sizeof(filter));
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }

    return S_OK;
}

void CDevice::Present()
{
    m_SwapChain->Present(0, 0);
}

void CDevice::Resize(Vec2 resolution)
{
    m_vRenderResolution = resolution;
    g_Global.g_RenderResolution = m_vRenderResolution;

    // ReSize 전 백버퍼가 참조하고있던 리소스 전부 Release
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"RenderTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"DepthStencilTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"FloatRenderTargetTexture");

    m_SwapChain->ResizeBuffers(0, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y, DXGI_FORMAT_UNKNOWN, 0);

    CreateBuffer();
}

void CDevice::SetViewport(float _Width, float _Height)
{
    D3D11_VIEWPORT ViewportDesc = {};

    ViewportDesc.MinDepth = 0;
    ViewportDesc.MaxDepth = 1.f;

    ViewportDesc.TopLeftX = 0;
    ViewportDesc.TopLeftY = 0;
    ViewportDesc.Width = _Width;
    ViewportDesc.Height = _Height;

    CONTEXT->RSSetViewports(1, &ViewportDesc);
}

int CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC tSamDesc = {};
    ZeroMemory(&tSamDesc, sizeof(tSamDesc));

    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.MinLOD = 0;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    tSamDesc.MinLOD = 0;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.MinLOD = 0;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.MinLOD = 0;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DEVICE->CreateSamplerState(&tSamDesc, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    tSamDesc.MinLOD = 0;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    tSamDesc.BorderColor[0] = 100000.f; // 큰 Z값
    DEVICE->CreateSamplerState(&tSamDesc, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());

    tSamDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    tSamDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    tSamDesc.MinLOD = 0;
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    tSamDesc.BorderColor[0] = 100000.f; // 큰 Z값
    DEVICE->CreateSamplerState(&tSamDesc, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::LINEAR_WRAP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::LINEAR_WRAP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::LINEAR_WRAP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::LINEAR_WRAP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::LINEAR_WRAP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());
    CONTEXT->CSSetSamplers((UINT)SS_TYPE::LINEAR_WRAP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_WRAP].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::LINEAR_CLAMP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::LINEAR_CLAMP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::LINEAR_CLAMP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::LINEAR_CLAMP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::LINEAR_CLAMP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());
    CONTEXT->CSSetSamplers((UINT)SS_TYPE::LINEAR_CLAMP, 1, m_arrSS[(UINT)SS_TYPE::LINEAR_CLAMP].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::POINT, 1, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::POINT, 1, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::POINT, 1, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::POINT, 1, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::POINT, 1, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());
    CONTEXT->CSSetSamplers((UINT)SS_TYPE::POINT, 1, m_arrSS[(UINT)SS_TYPE::POINT].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());
    CONTEXT->CSSetSamplers((UINT)SS_TYPE::ANISOTROPIC, 1, m_arrSS[(UINT)SS_TYPE::ANISOTROPIC].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::SHADOW_POINT, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::SHADOW_POINT, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::SHADOW_POINT, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::SHADOW_POINT, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::SHADOW_POINT, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());
    CONTEXT->CSSetSamplers((UINT)SS_TYPE::SHADOW_POINT, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_POINT].GetAddressOf());

    CONTEXT->VSSetSamplers((UINT)SS_TYPE::SHADOW_COMPARISION, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());
    CONTEXT->HSSetSamplers((UINT)SS_TYPE::SHADOW_COMPARISION, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());
    CONTEXT->DSSetSamplers((UINT)SS_TYPE::SHADOW_COMPARISION, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());
    CONTEXT->GSSetSamplers((UINT)SS_TYPE::SHADOW_COMPARISION, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());
    CONTEXT->PSSetSamplers((UINT)SS_TYPE::SHADOW_COMPARISION, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());
    CONTEXT->CSSetSamplers((UINT)SS_TYPE::SHADOW_COMPARISION, 1, m_arrSS[(UINT)SS_TYPE::SHADOW_COMPARISION].GetAddressOf());

    return S_OK;
}

int CDevice::CreateSwapChain()
{
    // SwapChain 생성 구조체
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    // SwapChain 이 관리하는 Buffer(RenderTarget)의 구성 정보
    tDesc.BufferCount = 2;
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tDesc.BufferDesc.RefreshRate.Denominator = 1;
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Windowed = true; // 창모드
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

    g_Global.g_RenderResolution = m_vRenderResolution;

    return S_OK;
}

int CDevice::CreateBuffer()
{
    // RenderTarget 용 텍스쳐 등록
    ComPtr<ID3D11Texture2D> tex2D;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());
    CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

    // FLOAT RenderTarget
    CAssetMgr::GetInst()->CreateTexture(L"FloatRenderTargetTexture", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y,
                                        DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    // DepthStencil 용도 텍스쳐 생성
    CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT,
                                        D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

    return S_OK;
}

int CDevice::CreateRasterizerState()
{
    HRESULT result = S_OK;

    D3D11_RASTERIZER_DESC tDesc = {};

    tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    tDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    tDesc.FrontCounterClockwise = false;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_BACK].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.FrontCounterClockwise = true;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_BACK_CCW].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.CullMode = D3D11_CULL_FRONT;
    tDesc.FillMode = D3D11_FILL_SOLID;
    tDesc.FrontCounterClockwise = false;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.FrontCounterClockwise = true;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT_CCW].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.CullMode = D3D11_CULL_NONE;
    tDesc.FillMode = D3D11_FILL_SOLID;
    tDesc.FrontCounterClockwise = false;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.FrontCounterClockwise = true;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE_CCW].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.CullMode = D3D11_CULL_NONE;
    tDesc.FillMode = D3D11_FILL_WIREFRAME;
    tDesc.FrontCounterClockwise = false;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    tDesc.FrontCounterClockwise = true;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME_CCW].GetAddressOf());
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
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
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

    // Stencil Mask
    tDesc.DepthEnable = true; // 이미 그려진 물체 유지
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    tDesc.DepthFunc = D3D11_COMPARISON_LESS;
    tDesc.StencilEnable = true;    // Stencil 필수
    tDesc.StencilReadMask = 0xFF;  // 모든 비트 다 사용
    tDesc.StencilWriteMask = 0xFF; // 모든 비트 다 사용
    tDesc.FrontFace.StencilFailOp = tDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    tDesc.FrontFace.StencilDepthFailOp = tDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    tDesc.FrontFace.StencilPassOp = tDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    tDesc.FrontFace.StencilFunc = tDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::MASK].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    // Stencil에 표기된 경우에"만" 그리는 DSS
    // DepthBuffer는 초기화된 상태로 가정
    tDesc.DepthEnable = true;   // 거울 속을 다시 그릴때 필요
    tDesc.StencilEnable = true; // Stencil 사용
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    tDesc.FrontFace.StencilFailOp = tDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    tDesc.FrontFace.StencilDepthFailOp = tDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    tDesc.FrontFace.StencilPassOp = tDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    tDesc.FrontFace.StencilFunc = tDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL; // Stencil에 표시된 부분만 그린다.

    hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::DRAW_MASKED].GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    return S_OK;
}

int CDevice::CreateBlendState()
{
    m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

    D3D11_BLEND_DESC tDesc = {};

    // Mask
    tDesc.AlphaToCoverageEnable = true;
    tDesc.IndependentBlendEnable = false;

    tDesc.RenderTarget[0].BlendEnable = true;
    tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

    tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::MASK].GetAddressOf());

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

    // One One
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

    // Decal
    tDesc = {};
    tDesc.AlphaToCoverageEnable = false;
    tDesc.IndependentBlendEnable = true;

    // PositionTarget
    tDesc.RenderTarget[0].BlendEnable = true;
    tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

    tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Normal
    tDesc.RenderTarget[1].BlendEnable = false;
    tDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
    tDesc.RenderTarget[1].DestBlend = D3D11_BLEND_ZERO;

    tDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
    tDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Emissive
    tDesc.RenderTarget[2].BlendEnable = true;
    tDesc.RenderTarget[2].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[2].SrcBlend = D3D11_BLEND_ONE;
    tDesc.RenderTarget[2].DestBlend = D3D11_BLEND_ONE;

    tDesc.RenderTarget[2].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[2].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[2].DestBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[2].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::DECAL].GetAddressOf());

    return S_OK;
}

int CDevice::CreateConstBuffer()
{
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
    m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

    m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST] = new CConstBuffer(CB_TYPE::MATERIAL_CONST);
    m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST]->Create(sizeof(tMtrlConst), 1);

    m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA] = new CConstBuffer(CB_TYPE::GLOBAL_DATA);
    m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA]->Create(sizeof(tGlobalData), 1);

    m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA] = new CConstBuffer(CB_TYPE::ANIM2D_DATA);
    m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA]->Create(sizeof(tAnimData2D), 1);

    return S_OK;
}
