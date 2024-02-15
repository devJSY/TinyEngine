#pragma once
#include "CAsset.h"

class CTexture : public CAsset
{
private:
    ScratchImage m_Image;            // 텍스쳐 로딩 및 시스템메모리 관리
    ComPtr<ID3D11Texture2D> m_Tex2D; // 텍스쳐 데이터를 GPU 메모리에 관리
    D3D11_TEXTURE2D_DESC m_Desc;     // 텍스쳐 생성 정보

    ComPtr<ID3D11RenderTargetView> m_RTV;   // 렌더타겟 용도
    ComPtr<ID3D11DepthStencilView> m_DSV;   // 뎊스 스텐실 용도
    ComPtr<ID3D11ShaderResourceView> m_SRV; // 쉐이더에서 사용하는 용도(텍스쳐 레지스터(t) 바인딩)
    ComPtr<ID3D11UnorderedAccessView>
        m_UAV; // GPGPU(General Purpose GPU) - ComputeShader, 읽기 쓰기 동시가능, (Unordered Register(u) 에 바인딩 가능)

    UINT m_RecentNum_SRV;
    UINT m_RecentNum_UAV;

private:
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage,
               const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc, const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc,
               const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc);
    int Create(ComPtr<ID3D11Texture2D> _tex2D, const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc,
               const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc,
               const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc);

private:
    virtual int Load(const wstring& _strFilePath) override;

public:
    void UpdateData(int _RegisterNum);

    int UpdateData_CS_SRV(int _RegisterNum);
    int UpdateData_CS_UAV(int _RegisterNum);

    static void Clear(int _iRegisterNum);
    void Clear_CS_SRV();
    void Clear_CS_UAV();

    UINT GetWidth() const { return m_Desc.Width; }
    UINT GetHeight() const { return m_Desc.Height; }

    ComPtr<ID3D11Texture2D> GetTex2D() const { return m_Tex2D; }
    const D3D11_TEXTURE2D_DESC& GetDesc() const { return m_Desc; }
    ComPtr<ID3D11RenderTargetView> GetRTV() const { return m_RTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() const { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() const { return m_UAV; }

    tPixel* GetPixels();
    void CaptureTex();

public:
    CTexture();
    virtual ~CTexture();

    friend class CAssetMgr;
};
