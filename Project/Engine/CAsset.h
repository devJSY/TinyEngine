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

    ASSET_TYPE GetComponentType() const { return m_Type; }

    // 파일로 부터 로딩
    virtual int Load(const wstring& _strFilePath) = 0;

public:
    // 파일로 저장
    virtual int Save(const wstring& _strFilePath) = 0;

public:
    CAsset(ASSET_TYPE _Type);
    virtual ~CAsset();

    friend class CAssetMgr;

    template <typename T>
    friend class Ptr;
};
