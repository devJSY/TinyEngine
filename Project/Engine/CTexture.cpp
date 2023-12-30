#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"
#include "CAssetMgr.h"

CTexture::CTexture()
    : CAsset(ASSET_TYPE::TEXTURE)
    , m_Desc{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const wstring& _strFilePath)
{
    // 확장자명 추출
    wchar_t szExt[20] = {};
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

    HRESULT hr = S_OK;

    if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }

    else
    {
        // png, bmp, jpg, jpeg
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
    {
        return E_FAIL;
    }

    // 1. ID3D11Texture2D 객체 생성
    // 2. ScratchImage 에 있는 데이터를 ID3D11Texture2D 객체 로 전달
    // 3. ID3D11Texture2D 를 이용한 ShaderResourceView 만들기
    CreateShaderResourceView(DEVICE, m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(),
                             m_SRV.GetAddressOf());

    // 만들어진 ShaderResourceView 를 이용해서 생성된 ID3D11Texture2D 객체를 알아낸다.
    m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

    // ID3D11Texture2D 객체를 통해서 생성될때 사용된 옵션(DESC) 정보를 가져온다.
    m_Tex2D->GetDesc(&m_Desc);

    return S_OK;
}

void CTexture::UpdateData(int _RegisterNum)
{
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Clear(int _iRegisterNum)
{
    Ptr<CTexture> pMissingTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"missing_texture");
    CONTEXT->VSSetShaderResources(_iRegisterNum, 1, pMissingTex->GetSRV().GetAddressOf());
    CONTEXT->HSSetShaderResources(_iRegisterNum, 1, pMissingTex->GetSRV().GetAddressOf());
    CONTEXT->DSSetShaderResources(_iRegisterNum, 1, pMissingTex->GetSRV().GetAddressOf());
    CONTEXT->GSSetShaderResources(_iRegisterNum, 1, pMissingTex->GetSRV().GetAddressOf());
    CONTEXT->PSSetShaderResources(_iRegisterNum, 1, pMissingTex->GetSRV().GetAddressOf());
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _tex2D)
{
    m_Tex2D = _tex2D;

    m_Tex2D->GetDesc(&m_Desc);

    // 바인드 플래그에 맞는 View 를 생성해준다.
    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
        {
            return E_FAIL;
        }
    }
    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }

        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }

        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }
    }

    return S_OK;
}
