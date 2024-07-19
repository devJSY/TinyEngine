#pragma once
#include "CMonsterUnitScript.h"

enum class GHOSTGORDO_STATE
{
    CloseEyeIdle,
    OpenEyeIdle,
    OpenEye,
    Find,
    Track,
    TrackAfter,
    End,
};

class CGhostGordoScript : public CMonsterUnitScript
{
private:
    GHOSTGORDO_STATE m_eState;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void ChangeState(GHOSTGORDO_STATE _state);
    void EnterState(GHOSTGORDO_STATE _state);
    void ExitState(GHOSTGORDO_STATE _state);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void CloseEyeIdle();
    void OpenEyeIdle();
    void OpenEye();
    void Find();
    void Track();
    void TrackAfter();

public:
    CLONE(CGhostGordoScript)
    CGhostGordoScript();
    CGhostGordoScript(const CGhostGordoScript& _Origin);
    virtual ~CGhostGordoScript();
};
