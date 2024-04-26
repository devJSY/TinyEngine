#pragma once
#include <Engine\\CScript.h>

class CPhysicsTestScript : public CScript
{
private:
    Vec3 m_Force;
    Vec3 m_Torque;
    int m_ForceMode;

public:
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider) override;
    virtual void OnCollisionStay(CCollider* _OtherCollider) override;
    virtual void OnCollisionExit(CCollider* _OtherCollider) override;

    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPhysicsTestScript);

public:
    CPhysicsTestScript();
    virtual ~CPhysicsTestScript();
};
