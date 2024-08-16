#pragma once
#include <Engine\CScript.h>

enum class PushOutColliderState
{
    MoveDest,
    MoveBase,
    Stop,
    End,
};

class CPushOutColliderScript : public CScript
{
public:
    CGameObject* m_pPlayer;
    Vec3 m_vBase;
    Vec3 m_vDir;
    PushOutColliderState m_eState;
    float m_fSpeed;
    float m_fReturnSpeed;
    bool m_bFlag;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetDestSpeed(const float _value) { m_fSpeed = _value; }
    void SetBaseSpeed(const float _value) { m_fReturnSpeed = _value; }
    void SetDir(const Vec3 _value) { m_vDir = _value; }
 
    CGameObject* CheckPlayer() { return m_pPlayer; }

    PushOutColliderState GetState() { return m_eState; }
    void SetState(const PushOutColliderState _state) { m_eState = _state; }

private:
    void MoveBase();
    void MoveDest();

    void Stop();

private:
    void Move(Vec3 _vDir, float _fSpeed);

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CPushOutColliderScript)

public:
    CPushOutColliderScript();
    CPushOutColliderScript(const CPushOutColliderScript& Origin);
    virtual ~CPushOutColliderScript();
};
