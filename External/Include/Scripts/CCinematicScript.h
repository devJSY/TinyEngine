#pragma once
#include <Engine\\CScript.h>

class CCinematicScript : public CScript
{
public:
    virtual void tick() override;

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CCinematicScript);

public:
    CCinematicScript();
    virtual ~CCinematicScript();
};
