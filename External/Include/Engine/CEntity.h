#pragma once

class CEntity
{
private:
    static UINT g_NextID;

private:
    const UINT m_ID;
    wstring m_strName;

public:
    void SetName(const wstring& _strName) { m_strName = _strName; }
    const wstring& GetName() const { return m_strName; }
    UINT GetID() const { return m_ID; }

public:
    virtual CEntity* Clone() = 0;

public:
    CEntity();
    CEntity(const CEntity& origin);
    virtual ~CEntity();
};
