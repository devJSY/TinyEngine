#pragma once
#include <Engine\CScript.h>

enum class CrumbleState
{
    Wait,
    DisppearWaitTime,
    PreDisappear,
    Disapper,
    Appear,
    End,
};

class CCrumbleScript : public CScript
{
private:
    CGameObject* m_pPlayer;
    CrumbleState m_eState;

    float m_fWaitTime;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void PreDisappear();
    void Disappear();
    void Appear();
    
private:
    void ChangeState(CrumbleState _state);
    void EnterState(CrumbleState _state);
    void ExitState(CrumbleState _state);

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CCrumbleScript);

public:
    CCrumbleScript();
    CCrumbleScript(const CCrumbleScript& Origin);
    virtual ~CCrumbleScript();
};
