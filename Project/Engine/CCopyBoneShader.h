#pragma once
#include "CComputeShader.h"

class CCopyBoneShader : public CComputeShader
{
private:
    CStructuredBuffer* m_SrcBuffer;
    CStructuredBuffer* m_DestBuffer;

public:
    // g_int_0 : BonCount
    void SetBoneCount(UINT _iBoneCount) { m_Const.arrInt[0] = _iBoneCount; }

    // g_int_1 : RowIndex
    void SetRowIndex(UINT _iRowIdx) { m_Const.arrInt[1] = _iRowIdx; }

    void SetSourceBuffer(CStructuredBuffer* _pBuffer) { m_SrcBuffer = _pBuffer; }
    void SetDestBuffer(CStructuredBuffer* _pBuffer) { m_DestBuffer = _pBuffer; }

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CCopyBoneShader();
    virtual ~CCopyBoneShader();
};
