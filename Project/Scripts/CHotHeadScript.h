#pragma once
#include "CMonsterUnitScript.h"
enum class HOTHEAD_STATE
{
    Idle,
    Damage,
    End,
};


class CHotHeadScript : public CMonsterUnitScript
{
private:
    HOTHEAD_STATE m_eState;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState(HOTHEAD_STATE _state);
    void ChangeState(HOTHEAD_STATE _state);
    void ExitState(HOTHEAD_STATE _state);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CHotHeadScript)
    CHotHeadScript();
    virtual ~CHotHeadScript();
};
