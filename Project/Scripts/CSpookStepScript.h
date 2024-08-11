#pragma once

#include "CMonsterUnitScript.h"
enum class SpookStepState
{
};

class CSpookStepScript : public CMonsterUnitScript
{
private:
    SpookStepState m_eState;
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
    void ChangeState(SpookStepState _state);
    void EnterState(SpookStepState _state);
    void ExitState(SpookStepState _state);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CSpookStepScript)

public:
    CSpookStepScript();
    CSpookStepScript(const CSpookStepScript& Origin);
    virtual ~CSpookStepScript();
};
