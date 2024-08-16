#pragma once

#include "CStructuredBuffer.h"
#include "CTexture.h"
#include "CCopyBoneShader.h"

class CInstancingBuffer : public CSingleton<CInstancingBuffer>
{
private:
    ComPtr<ID3D11Buffer> m_InstancingBuffer;
    UINT m_MaxCount;
    vector<tInstancingData> m_vecData;

    // Anim3D ¿ëµµ
    vector<CStructuredBuffer*> m_vecBoneMat;
    vector<CStructuredBuffer*> m_vecPrevBoneMat;
    CStructuredBuffer* m_BoneBuffer;
    CStructuredBuffer* m_PrevBoneBuffer;

    Ptr<CCopyBoneShader> m_CopyShader;

public:
    void init();
    void Clear()
    {
        m_vecData.clear();
        m_vecBoneMat.clear();
        m_vecPrevBoneMat.clear();
    }
    void AddInstancingData(tInstancingData& _tData) { m_vecData.push_back(_tData); }

    UINT GetInstanceCount() const { return (UINT)m_vecData.size(); }
    ComPtr<ID3D11Buffer> GetBuffer() const { return m_InstancingBuffer; }
    void SetData();

    void AddInstancingBoneMat(CStructuredBuffer* _pBuffer, CStructuredBuffer* _pPrevBuffer);
    CStructuredBuffer* GetBoneBuffer() const { return m_BoneBuffer; }
    CStructuredBuffer* GetPrevBoneBuffer() const { return m_PrevBoneBuffer; }

private:
    void Resize(UINT _iCount);

public:
    CInstancingBuffer();
    virtual ~CInstancingBuffer();
};
