#pragma once
#include <Engine\\CScript.h>

class CCameraLockScript : public CScript
{
public:
    virtual void tick() override;

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CCameraLockScript);

public:
    CCameraLockScript();
    virtual ~CCameraLockScript();
};
