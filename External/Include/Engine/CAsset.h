#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CAsset : public CEntity
{
private:
    wstring m_Key;
    wstring m_RelativePath;
    const ASSET_TYPE m_Type;
    int m_RefCount;
    const bool m_bEngineAsset;

public:
    const wstring& GetKey() const { return m_Key; }
    const wstring& GetRelativePath() const { return m_RelativePath; }

public:
    ASSET_TYPE GetAssetType() const { return m_Type; }
    int GetRefCount() const { return m_RefCount; }

protected:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _RelativePath) { m_RelativePath = _RelativePath; }

private:
    void AddRef() 
    { 
        ++m_RefCount;
    }
    void Release()
    {
        --m_RefCount;
        if (0 >= m_RefCount)
        {
            delete this;
        }
    }

public:
    // 파일로 저장
    virtual int Save(const wstring& _strFilePath) = 0;

private:
    // 파일로 부터 로딩
    virtual int Load(const wstring& _strFilePath) = 0;

public:
    CAsset(ASSET_TYPE _Type, bool _bEngineAsset = false);
    virtual ~CAsset();

    friend class CAssetMgr;

    template <typename T>
    friend class Ptr;
};
