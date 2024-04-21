#pragma once
#include "CShader.h"

#include "CStructuredBuffer.h"
#include "CTexture.h"

class CComputeShader : public CShader
{
private:
    ComPtr<ID3DBlob> m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;

    // 그룹 개수
    UINT m_GroupX;
    UINT m_GroupY;
    UINT m_GroupZ;

protected:
    // 스레드 개수
    const UINT m_ThreadX;
    const UINT m_ThreadY;
    const UINT m_ThreadZ;

    tMtrlConst m_Const;

public:
    int Create(const wstring& _strRelativePath, const string& _strFuncName);

public:
    void Execute();

protected:
    UINT GetGroupX() const { return m_GroupX; }
    UINT GetGroupY() const { return m_GroupY; }
    UINT GetGroupZ() const { return m_GroupZ; }

    void SetGroupX(UINT _Group) { m_GroupX = _Group; }
    void SetGroupY(UINT _Group) { m_GroupY = _Group; }
    void SetGroupZ(UINT _Group) { m_GroupZ = _Group; }

    void SetGroup(UINT _X, UINT _Y, UINT _Z)
    {
        m_GroupX = _X;
        m_GroupY = _Y;
        m_GroupZ = _Z;
    }

private:
    virtual int UpdateData() = 0;
    virtual void UpdateGroupCount() = 0;
    virtual void Clear() = 0;

public:
    CComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ);
    virtual ~CComputeShader();
};
