
#include <Engine\CScript.h>

enum class MonsterDeadEffectSpawnState
{
    Enter,
    End,
};

class CMonsterDeadEffectSpawnScript : public CScript
{
private:
    MonsterDeadEffectSpawnState m_eState;
    int m_iBodySize;

public:
    virtual void tick() override;

private:
    void ChangeState(MonsterDeadEffectSpawnState _eState) { m_eState = _eState; }
    void Enter();
    void End();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMonsterDeadEffectSpawnScript)
public:
    CMonsterDeadEffectSpawnScript();
    CMonsterDeadEffectSpawnScript(const CMonsterDeadEffectSpawnScript& Origin);
    virtual ~CMonsterDeadEffectSpawnScript();
};
