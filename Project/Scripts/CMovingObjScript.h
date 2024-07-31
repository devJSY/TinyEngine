#pragma once
#include <Engine\CScript.h>

enum class MovingObjState
{
    Move,
    Stop,
    End,
};

class CMovingObjScript : public CScript
{
private:
    Vec3 m_vDest;
    Vec3 m_vDir;
    Vec3 m_vOriginPos;
    MovingObjState m_eState;
    float m_fSpeed;
    bool m_bLoop;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    MovingObjState GetState() { return m_eState; }
    void SetState(const MovingObjState _state) { m_eState = _state; }

private:
    void Move();
    void Stop();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMovingObjScript);

public:
    CMovingObjScript();
    CMovingObjScript(const CMovingObjScript& Origin);
    virtual ~CMovingObjScript();
};