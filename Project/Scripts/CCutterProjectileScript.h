#pragma once
#include "CMonsterATKScript.h"

enum class CUTTERPROJECTILE_STATE
{
    Attack,
    AttackStop,
    AttackBack,
    Destroy,
    End,
};

class CCutterProjectileScript : public CMonsterATKScript
{
private:
    CGameObject* m_pOwner;
    CUTTERPROJECTILE_STATE m_eState;
    Quat m_Quat;
    Vec3 m_vOriginPos;
    float m_fSpeed;
    float m_fAccTime;

public:
    void SetOwenr(CGameObject* _pObj) { m_pOwner = _pObj; }
    void SetQuaternion(const Quat& _quat) { m_Quat = _quat; }

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void EnterState(CUTTERPROJECTILE_STATE _state);
    void ChangeState(CUTTERPROJECTILE_STATE _state);
    void ExitState(CUTTERPROJECTILE_STATE _state);

private:
    void Attack();
    void AttackStop();
    void AttackBack();
    void Destroy();

private:
    CLONE(CCutterProjectileScript)
    CCutterProjectileScript();
    CCutterProjectileScript(const CCutterProjectileScript& _Origin);
    virtual ~CCutterProjectileScript();
};
