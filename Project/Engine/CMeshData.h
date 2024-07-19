#pragma once
#include "CAsset.h"

class CGameObject;
class CGameObjectEx;

class CMeshData : public CAsset
{
private:
    Ptr<CMesh> m_pMesh;
    vector<Ptr<CMaterial>> m_vecMtrl;

public:
    static CMeshData* LoadFromFBX(const wstring& _RelativePath);

    CGameObject* Instantiate();
    CGameObjectEx* InstantiateEx();

    Ptr<CMesh> GetMesh() const { return m_pMesh; }
    const vector<Ptr<CMaterial>>& GetVecMaterial() const { return m_vecMtrl; }

public:
    virtual int Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

    CLONE_DISABLE(CMeshData);

public:
    CMeshData(bool _Engine = false);
    virtual ~CMeshData();

    friend class CModelEditor;
};
