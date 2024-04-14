#pragma once
#include <Engine\\CScript.h>

class CExitElevatorScript : public CScript
{
private:
    bool m_bExit;
    bool m_bSoundPlayed;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CExitElevatorScript);

public:
    CExitElevatorScript();
    virtual ~CExitElevatorScript();
};
