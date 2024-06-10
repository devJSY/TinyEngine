#pragma once
#include "CAsset.h"

#include "CFBXLoader.h"

struct tIndexInfo
{
    ComPtr<ID3D11Buffer> pIB;
    D3D11_BUFFER_DESC tIBDesc;
    UINT iIdxCount;
    void* pIdxSysMem;
};

class CMesh : public CAsset
{
private:
    ComPtr<ID3D11Buffer> m_VB;
    D3D11_BUFFER_DESC m_VBDesc;
    UINT m_VtxCount;
    void* m_VtxSysMem;

    // 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
    vector<tIndexInfo> m_vecIdxInfo;

private:
    void UpdateData(UINT _iSubset);

public:
    UINT GetVtxCount() const { return m_VtxCount; }
    Vtx* GetVtxSysMem() const { return (Vtx*)m_VtxSysMem; }
    UINT GetSubsetCount() const { return (UINT)m_vecIdxInfo.size(); }

public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);

    void render(UINT _iSubset);
    void render_draw(UINT _iSubset);
    void render_IndexedInstanced(UINT _InstanceCount);

public:
    virtual int Save(const wstring& _strRelativePath) override { return E_FAIL; }
    virtual int Load(const wstring& _strFilePath) override { return E_FAIL; }

    CLONE_DISABLE(CMesh);

public:
    CMesh(bool _bEngineAsset = false);
    virtual ~CMesh();
};
