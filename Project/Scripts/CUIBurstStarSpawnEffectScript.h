#pragma once
#include <Engine\CScript.h>

enum class MonsterBurstStarSpawnState
{
    Enter,
    End,
};

class CUIBurstStarSpawnEffectScript : public CScript
{
private:
    MonsterBurstStarSpawnState m_eState;

public:
    virtual void tick() override;

private:
    void ChangeState(MonsterBurstStarSpawnState _eState) { m_eState = _eState; }
    void Enter();
    void End();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIBurstStarSpawnEffectScript)
public:
    CUIBurstStarSpawnEffectScript();
    CUIBurstStarSpawnEffectScript(const CUIBurstStarSpawnEffectScript& Origin);
    virtual ~CUIBurstStarSpawnEffectScript();
};
