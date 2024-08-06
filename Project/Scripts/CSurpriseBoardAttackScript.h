#pragma once
#include <Engine\CScript.h>
class CSurpriseBoardAttackScript : public CScript
{
private:
    float m_fDamage;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSurpriseBoardAttackScript)
public:
    CSurpriseBoardAttackScript();
    CSurpriseBoardAttackScript(const CSurpriseBoardAttackScript& Origin);
    virtual ~CSurpriseBoardAttackScript();
};
