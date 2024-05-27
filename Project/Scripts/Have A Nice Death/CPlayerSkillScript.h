#pragma once
#include <Engine\\CScript.h>

class CPlayerSkillScript : public CScript
{
private:
    int m_ATK;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerSkillScript);

public:
    CPlayerSkillScript();
    CPlayerSkillScript(const CPlayerSkillScript& origin);
    virtual ~CPlayerSkillScript();
};
