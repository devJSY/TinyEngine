#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CAsset : public CEntity
{
private:
    wstring m_Key;
    wstring m_RelativePath;
    int m_RefCount;
    const ASSET_TYPE m_Type;

public:
    const wstring& GetKey() const { return m_Key; }
    const wstring& GetRelativePath() const { return m_RelativePath; }

private:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _RelativePath) { m_RelativePath = _RelativePath; }

    void AddRef() { ++m_RefCount; }
    void Release()
    {
        --m_RefCount;
        if (0 >= m_RefCount)
        {
            delete this;
        }
    }

    int GetRefCount() const { return m_RefCount; }

    ASSET_TYPE GetType() const { return m_Type; }

    virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

public:
    CAsset(ASSET_TYPE _Type);
    virtual ~CAsset();

    friend class CAssetMgr;
    
    template <typename T>
    friend class Ptr;
};
