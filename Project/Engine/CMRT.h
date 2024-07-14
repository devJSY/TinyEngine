#pragma once
#include "CEntity.h"

class CMRT : public CEntity
{
private:
    Ptr<CTexture> m_arrRTTex[8];
    ID3D11RenderTargetView* m_arrRTView[8];
    Vec4 m_arrClearColor[8];

    UINT m_RTCount;

    Ptr<CTexture> m_DSTex;

    D3D11_VIEWPORT m_tViewPort;

public:
    void Create(Ptr<CTexture>* _pArrTex, Vec4* _ClearColor, UINT _RTCount, Ptr<CTexture> _DSTex);

    void OMSet();
    void Clear();

    Ptr<CTexture> GetRenderTargetTex(UINT _Idx) const { return m_arrRTTex[_Idx]; }
    ID3D11RenderTargetView* const* GetRenderTargetView() const { return m_arrRTView; }
    Ptr<CTexture> GetDepthStencilTex() const { return m_DSTex; }
    const D3D11_VIEWPORT& GetViewPort() const { return m_tViewPort; }

    UINT GetRTCount() const { return m_RTCount; }

    CLONE_DISABLE(CMRT);

public:
    CMRT();
    virtual ~CMRT();
};
