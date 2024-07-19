#pragma once
#include "CMonsterATKScript.h"

enum class FIREPROJECTILE_STATE
{
    Attack,
    Destroy,
    End,
};

class CFireProjectileScript : public CMonsterATKScript
{
private:
    FIREPROJECTILE_STATE m_eState;
    Quat m_Quat;
    float m_fSpeed;
    float m_fFallSpeed;
    float m_fAccRadian;

public:
    void SetQuaternion(const Quat& _quat) { m_Quat = _quat; }
    void SetFallSpeed(const float _fFallSpeed) { m_fFallSpeed = _fFallSpeed; }
    void SetSpeed(const float _fSpeed) { m_fSpeed = _fSpeed; }

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
    void EnterState(FIREPROJECTILE_STATE _state);
    void ChangeState(FIREPROJECTILE_STATE _state);
    void ExitState(FIREPROJECTILE_STATE _state);

private:
    void Attack();
    void Destroy();

private:
    CLONE(CFireProjectileScript)
    CFireProjectileScript();
    CFireProjectileScript(const CFireProjectileScript& _Origin);
    virtual ~CFireProjectileScript();
};
