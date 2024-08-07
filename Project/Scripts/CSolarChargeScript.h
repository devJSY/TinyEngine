#pragma once
#include <Engine\CScript.h>

enum class SolarChargeState
{
    OffWait,
    ChargeOn,
    ChargeOff,
    ChargedStart,
    ChargedWait,
    Decreases,
    OffWaitStart,
    End,
};

class CSolarChargeScript : public CScript
{
private:
    string m_MovingObjName;
    SolarChargeState m_eState;
    bool m_IsArea;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(SolarChargeState _state);
    void EnterState();
    void ExitState();

private:
    void OffWait();
    void ChargeOn();
    void ChargeOff();
    void ChargedStart();
    void ChargedWait();
    void Decrease();
    void OffWaitStart();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSolarChargeScript)
public:
    CSolarChargeScript();
    CSolarChargeScript(const CSolarChargeScript& Origin);
    virtual ~CSolarChargeScript();
};
