#pragma once
#include <Engine\CScript.h>

enum class SolarOnceState
{
    ChargeOn,
    ChargeOff,
    OnWait,
    OnWaitStart,
    OffWait,
    End,
};

class CSolarOnceScript : public CScript
{
private:
    string m_MovingObjName;
    SolarOnceState m_eState;
    bool m_IsArea;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

private:
    void ChangeState(SolarOnceState _state);
    void EnterState(SolarOnceState _state);
    void ExitState(SolarOnceState _state);

private:
    void ChargeOn();
    void ChargeOff();
    void OnWaitStart();
    void OffWait();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSolarOnceScript)
public:
    CSolarOnceScript();
    CSolarOnceScript(const CSolarOnceScript& Origin);
    virtual ~CSolarOnceScript();
};
