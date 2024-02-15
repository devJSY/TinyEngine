#pragma once
#include "CAsset.h"
class CShader : public CAsset
{
protected:
    ComPtr<ID3DBlob> m_ErrBlob;
    UINT m_CompileFlags;

public:
    virtual int UpdateData() = 0;

public:
    CShader(ASSET_TYPE _Type);
    virtual ~CShader();
};
