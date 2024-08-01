#pragma once
#include <Engine\CScript.h>

enum class DisappearObjectState
{
    Wait,
    PreDisappear,
    Disapper,
    Appear,
    End,
};

class CDisappearObjScript : public CScript
{
private:
    CGameObject* m_pPlayer;
    DisappearObjectState m_eState;
    float m_fAccTime;
    float m_fBreakTime;
    float m_fCreateTime;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void PreDisappear();
    void Disappear();
    void Appear();

private:
    void ChangeState(DisappearObjectState _state);
    void EnterState(DisappearObjectState _state);
    void ExitState(DisappearObjectState _state);

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CDisappearObjScript);

public:
    CDisappearObjScript();
    CDisappearObjScript(const CDisappearObjScript& Origin);
    virtual ~CDisappearObjScript();
};
