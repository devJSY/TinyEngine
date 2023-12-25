#pragma once
#include "CAsset.h"

class CMesh : public CAsset
{
private:
    ComPtr<ID3D11Buffer> m_VB;
    ComPtr<ID3D11Buffer> m_IB;

    D3D11_BUFFER_DESC m_VBDesc;
    D3D11_BUFFER_DESC m_IBDesc;

    UINT m_VtxCount;
    UINT m_IdxCount;

    void* m_VtxSysMem;
    void* m_IdxSysMem;

private:
    std::wstring m_AlbedoTextureName;
    std::wstring m_AoTextureName;
    std::wstring m_NormalTextureName;
    std::wstring m_HeightTextureName;
    std::wstring m_MetallicRoughnessTextureName;
    std::wstring m_EmissiveTextureName;

public:
    void SetAlbedoTexture(const std::wstring& name) { m_AlbedoTextureName = name; }
    void SetAmbientOcclusionTexture(const std::wstring& name) { m_AoTextureName = name; }
    void SetNormalTexture(const std::wstring& name) { m_NormalTextureName = name; }
    void SetHeightTexture(const std::wstring& name) { m_HeightTextureName = name; }
    void SetMetallicRoughnessTexture(const std::wstring& name) { m_MetallicRoughnessTextureName = name; }
    void SetEmissiveTexture(const std::wstring& name) { m_EmissiveTextureName = name; }

private:
    void UpdateData();
    void TextureBind();

public:
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);
    void render();
    void renderDraw();

public:
    CMesh();
    virtual ~CMesh();
};
