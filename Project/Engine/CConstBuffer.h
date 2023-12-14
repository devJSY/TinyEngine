#pragma once
#include "CEntity.h"

class CConstBuffer : public CEntity
{
private:
    D3D11_BUFFER_DESC m_Desc;
    ComPtr<ID3D11Buffer> m_CB;

    UINT m_ElementSize;
    UINT m_ElementCount;

public:
    int Create(UINT _ElementSize, UINT _ElementCount);
    void SetData(void* _Src, UINT _ElementCount = 0);
    void UpdateData(UINT _RegisterNum);

public:
    CConstBuffer();
    ~CConstBuffer();
};
