#pragma once
#include <Engine\CScript.h>

enum class CPushOutTriggerState
{
    Wait,
    Stop,
    End,
};

class CPushOutTriggerScript : public CScript
{
private:
    CGameObject* m_pPushOutObj;
    CPushOutTriggerState m_eState;
    int m_iPushOutNum;
    bool m_bEnabled;
    float m_fWaitTime;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(CPushOutTriggerState _state);
    void EnterState();
    void ExitState();

private:
    void Stop();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CPushOutTriggerScript)

public:
    CPushOutTriggerScript();
    CPushOutTriggerScript(const CPushOutTriggerScript& Origin);
    virtual ~CPushOutTriggerScript();
};
