#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class CStructuredBuffer : public CEntity
{
private:
    ComPtr<ID3D11Buffer> m_SB;
    ComPtr<ID3D11ShaderResourceView> m_SRV;

    ComPtr<ID3D11Buffer> m_SB_Read;
    ComPtr<ID3D11Buffer> m_SB_Write;

    UINT m_ElementSize;
    UINT m_ElementCount;

    SB_TYPE m_Type;
    bool m_bSysMemMove;

public:
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove = false,
               void* _pSysMem = nullptr);
    void UpdateData(UINT _RegisterNum);
    void SetData(void* _SysMem, UINT _ElementCount = 0);
    void GetData(void* _Dest, UINT _ElementCount = 0);

    UINT GetElementSize() const { return m_ElementSize; }
    UINT GetElementCount() const { return m_ElementCount; }

public:
    CStructuredBuffer();
    virtual ~CStructuredBuffer();
};
