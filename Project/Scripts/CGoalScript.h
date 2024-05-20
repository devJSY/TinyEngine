#pragma once
#include <Engine\\CScript.h>

class CGoalScript : public CScript
{
public:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CGoalScript);

public:
    CGoalScript();
    virtual ~CGoalScript();
};
