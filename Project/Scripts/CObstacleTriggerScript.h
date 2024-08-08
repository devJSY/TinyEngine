#pragma once
#include <Engine\CScript.h>

enum class CObstacleTriggerState
{
    Wait,
    Stop,
    End,
};

class CObstacleTriggerScript : public CScript
{
private:
    CGameObject* m_pSurPrisedObj[2];
    CObstacleTriggerState m_eState;
    int m_iSurPrisedNum[2];
    float m_fWaitTime;
    float m_fAccTime;
    bool m_bChainAction;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(CObstacleTriggerState _state);
    void EnterState();
    void ExitState();

private:
    void Stop();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CObstacleTriggerScript)

public:
    CObstacleTriggerScript();
    CObstacleTriggerScript(const CObstacleTriggerScript& Origin);
    virtual ~CObstacleTriggerScript();
};
