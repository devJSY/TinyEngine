#include "pch.h"
#include "CMRT.h"
#include "CDevice.h"

CMRT::CMRT()
    : m_arrRTTex{}
    , m_arrRTView{}
    , m_arrClearColor{}
    , m_RTCount(0)
    , m_DSTex(nullptr)
    , m_tViewPort()
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>* _pArrTex, Vec4* _ClearColor, UINT _RTCount, Ptr<CTexture> _DSTex)
{
    assert(_RTCount <= 8);

    m_RTCount = _RTCount;

    for (size_t i = 0; i < m_RTCount; i++)
    {
        m_arrRTTex[i] = _pArrTex[i];
        m_arrRTView[i] = m_arrRTTex[i]->GetRTV().Get();
        m_arrClearColor[i] = _ClearColor[i];
    }

    m_DSTex = _DSTex;

    // ViewPort 설정
    m_tViewPort.TopLeftX = 0;
    m_tViewPort.TopLeftY = 0;

    m_tViewPort.Width = (float)m_arrRTTex[0]->GetWidth();
    m_tViewPort.Height = (float)m_arrRTTex[0]->GetHeight();

    m_tViewPort.MinDepth = 0;
    m_tViewPort.MaxDepth = 1;
}

void CMRT::OMSet()
{
    if (nullptr != m_DSTex)
    {
        CONTEXT->OMSetRenderTargets(m_RTCount, m_arrRTView, m_DSTex->GetDSV().Get());
    }
    else
    {
        CONTEXT->OMSetRenderTargets(m_RTCount, m_arrRTView, nullptr);
    }

    // 뷰포트 설정
    CONTEXT->RSSetViewports(1, &m_tViewPort);
}

void CMRT::Clear()
{
    for (UINT i = 0; i < m_RTCount; ++i)
    {
        CONTEXT->ClearRenderTargetView(m_arrRTTex[i]->GetRTV().Get(), m_arrClearColor[i]);
    }

    if (nullptr != m_DSTex)
    {
        CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    }
}
