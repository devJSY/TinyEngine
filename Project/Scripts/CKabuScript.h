#pragma once
#include "CMonsterUnitScript.h"

enum class KabuState
{
    Patrol,
    Fall,
    Landing,
    Damage,
    Return,
    Eaten,
    Death,
    End,
};

class CKabuScript : public CMonsterUnitScript
{
private:
    KabuState m_eState;

    Vec3 m_vCenterPos;

    Vec3 m_vOriginPos;
    Vec3 m_vDestPos;

    Vec3 m_vDamageDir;
    Vec3 m_vDir;
    Vec3 m_vPrevDir;

    bool m_bInverse;
    bool m_bFlag;
    bool m_bCurved;
    bool m_bHalfCurved;

    bool m_bHalfFlag;

    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void EnterState(KabuState _state);
    void FSM();
    void ExitState(KabuState _state);
    void ChangeState(KabuState _state);
    void CheckDamage();

    void PatrolMove();
    void CircleMove();
    void LinearMove(bool _moveZ);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerStay(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void Patrol();
    void Fall();
    void Landing();
    void Damage();
    void Eaten();
    void Return();
    void Death();

public:
    CLONE(CKabuScript)
    CKabuScript();
    CKabuScript(const CKabuScript& Origin);
    virtual ~CKabuScript();
};
