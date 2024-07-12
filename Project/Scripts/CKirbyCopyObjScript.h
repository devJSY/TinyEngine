#pragma once
#include <Engine\\CScript.h>

class CKirbyCopyObjScript : public CScript
{
private:
    ObjectCopyType m_ObjectType;

public:
    ObjectCopyType GetObjType() const { return m_ObjectType; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyCopyObjScript);
    CKirbyCopyObjScript();
    CKirbyCopyObjScript(const CKirbyCopyObjScript& _Origin);
    virtual ~CKirbyCopyObjScript();
};