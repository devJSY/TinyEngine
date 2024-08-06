#pragma once
#include <Engine\CScript.h>

enum class ElevatorState
{
    Move,
    Stop,
    End,
};

class CElevatorScript : public CScript
{
private:
    CGameObject* m_pPlayer;
    Vec3 m_vDest;
    ElevatorState m_eState;
    float m_fSpeed;
    bool m_bFlag;

public:
    virtual void tick() override;

public:
    ElevatorState GetState() { return m_eState; }
    void SetState(const ElevatorState _state) { m_eState = _state; }

private:
    void Move();
    void Stop();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CElevatorScript);

public:
    CElevatorScript();
    CElevatorScript(const CElevatorScript& Origin);
    virtual ~CElevatorScript();
};