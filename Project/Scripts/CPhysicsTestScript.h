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

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPhysicsTestScript);

public:
    CPhysicsTestScript();
    virtual ~CPhysicsTestScript();
};
