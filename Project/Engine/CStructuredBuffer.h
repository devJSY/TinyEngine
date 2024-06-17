#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,  // SRV
    READ_WRITE, // SRV + UAV
};

class CStructuredBuffer : public CEntity
{
private:
    ComPtr<ID3D11Buffer> m_SB;
    ComPtr<ID3D11ShaderResourceView> m_SRV;
    ComPtr<ID3D11UnorderedAccessView> m_UAV;

    ComPtr<ID3D11Buffer> m_SB_Read;
    ComPtr<ID3D11Buffer> m_SB_Write;

    UINT m_ElementSize;
    UINT m_ElementCount;

    SB_TYPE m_Type;
    bool m_bSysMemMove;

    UINT m_RecentSRV; // 최근에 바인딩한 t레지스터 번호
    UINT m_RecentUAV; // 최근에 바인딩한 u레지스터 번호

public:
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove = false, void* _pSysMem = nullptr);
    void UpdateData(UINT _RegisterNum);
    int UpdateData_CS_SRV(UINT _RegisterNum);
    int UpdateData_CS_UAV(UINT _RegisterNum);

    void Clear(UINT _RegisterNum);
    void Clear_CS_SRV();
    void Clear_CS_UAV();

    void SetData(void* _SysMem, UINT _ElementCount = 0);
    void GetData(void* _Dest, UINT _ElementCount = 0);

    UINT GetElementSize() const { return m_ElementSize; }
    UINT GetElementCount() const { return m_ElementCount; }
    UINT GetBufferSize() const { return m_ElementSize * m_ElementCount; }

    CLONE(CStructuredBuffer);

public:
    CStructuredBuffer();
    CStructuredBuffer(const CStructuredBuffer& origin);
    virtual ~CStructuredBuffer();
};
