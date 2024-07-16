#pragma once
#include "CMonsterUnitScript.h"

enum class KABU_STATE
{
    Patrol,
    Damage,
    Death,
    End,
};

class CKabuScript : public CMonsterUnitScript
{
private:
    KABU_STATE m_eState;

    Vec3 m_vPatrolDir;
    Vec3 m_vOriginPos;
    Vec3 m_vDestPos;

    float m_fLerpValue;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void PatrolMove();

private:
    void ChangeState(KABU_STATE _state);
    void EnterState(KABU_STATE _state);
    void ExitState(KABU_STATE _state);

private:
    void Patrol();
    void Damage();
    void Death();

    Vec3 CaculateDir(Vec3 _vRadian);

public:
    CLONE(CKabuScript)
    CKabuScript();
    virtual ~CKabuScript();
};
