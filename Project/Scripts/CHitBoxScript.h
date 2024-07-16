#pragma once
#include <Engine\\CScript.h>

class CHitBoxScript : public CScript
{
private:
    bool m_IsinvincibilityObj;
    bool m_IsReflectionDamageObj;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CHitBoxScript);
    CHitBoxScript();
    virtual ~CHitBoxScript();
};
