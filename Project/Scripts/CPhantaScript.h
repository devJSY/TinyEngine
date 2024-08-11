#pragma once
#include "CMonsterUnitScript.h"

enum class PhantaState
{
};

class CPhantaScript : public CMonsterUnitScript
{
private:
    PhantaState m_eState;
    Quat m_qBaseQuat;
    Vec3 m_vBasePos;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void ChangeState(PhantaState _state);
    void EnterState(PhantaState _state);
    void ExitState(PhantaState _state);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CPhantaScript)

public:
    CPhantaScript();
    CPhantaScript(const CPhantaScript& Origin);
    virtual ~CPhantaScript();
};
