#pragma once
#include "CMonsterUnitScript.h"

enum class BLADEKNIGHTSWORD_STATE
{
    Thrust,
    ThrustEnd,
    ThrustLoop,
    ThrustStart,
    ThrustStartWait,
    // ThrustWait,
};

class CBladeKnightSwordScript : public CScript
{
private:
    BLADEKNIGHTSWORD_STATE m_State;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void ChangeState(BLADEKNIGHTSWORD_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Thrust();
    void ThrustEnd();
    void ThrustLoop();
    void ThrustStart();
    void ThrustStartWait();
    void ThrustWait();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CBladeKnightSwordScript);

public:
    CBladeKnightSwordScript();
    CBladeKnightSwordScript(const CBladeKnightSwordScript& origin);
    virtual ~CBladeKnightSwordScript();
};
