#pragma once
#include "CAsset.h"
class CShader : public CAsset
{
protected:
    ComPtr<ID3DBlob> m_ErrBlob;

public:
    virtual void UpdateData() = 0;

private:
    virtual int Load(const wstring& _strFilePath) override { return S_OK; };

public:
    virtual int Save(const wstring& _strFilePath) override { return S_OK; };

public:
    CShader(ASSET_TYPE _Type);
    virtual ~CShader();
};
