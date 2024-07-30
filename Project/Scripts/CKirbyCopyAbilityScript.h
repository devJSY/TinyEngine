#pragma once
#include <Engine\\CScript.h>

class CKirbyCopyAbilityScript : public CScript
{
private:
    UINT m_AbilityType;

public:
    void SetAbilityType(UINT _Type) { m_AbilityType = _Type; }
    void SetAbilityType(AbilityCopyType _Type) { m_AbilityType = (UINT)_Type; }
    AbilityCopyType GetAbilityType() const { return (AbilityCopyType)m_AbilityType; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyCopyAbilityScript);
    CKirbyCopyAbilityScript();
    CKirbyCopyAbilityScript(const CKirbyCopyAbilityScript& _Origin);
    virtual ~CKirbyCopyAbilityScript();
};
