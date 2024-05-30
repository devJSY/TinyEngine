#pragma once

#include "CComputeShader.h"
#include "CTexture.h"

class CStructuredBuffer;

class CRaycastShader : public CComputeShader
{
private:
    // ������ ���� ����
    Ptr<CTexture> m_pHeightMap;

    // ���� �� ����
    UINT m_iXFace;
    UINT m_iZFace;

    // ī�޶� Ray ����
    tRay m_Ray;

    // ������ġ ��� ����
    CStructuredBuffer* m_pOutput;

public:
    void SetFaceCount(UINT _x, UINT _z)
    {
        m_iXFace = _x;
        m_iZFace = _z;
    }
    void SetCameraRay(const tRay& _ray) { m_Ray = _ray; }
    void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }
    void SetHeightMap(Ptr<CTexture> _ptex) { m_pHeightMap = _ptex; }

protected:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CRaycastShader();
    virtual ~CRaycastShader();
};