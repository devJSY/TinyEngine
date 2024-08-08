#pragma once
#include "CMonsterUnitScript.h"

enum class KABU_STATE
{
    Patrol,
    Damage,
    Eaten,
    Death,
    End,
};

class CKabuScript : public CMonsterUnitScript
{
private:
    KABU_STATE m_eState;

    Vec3 m_vCenterPos;

    Vec3 m_vOriginPos;
    Vec3 m_vDestPos;

    Vec3 m_vDamageDir;
    bool m_bInverse;
    bool m_bFlag;
    bool m_bCurved;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void PatrolMove();

private:
    void ChangeState(KABU_STATE _state);
    void EnterState(KABU_STATE _state);
    void ExitState(KABU_STATE _state);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void Patrol();
    void Damage();
    void Eaten();
    void Death();

    Vec3 CaculateDir(Vec3 _vRadian);

public:
    CLONE(CKabuScript)
    CKabuScript();
    CKabuScript(const CKabuScript& _Origin);
    virtual ~CKabuScript();
};
