#pragma once
#include <Engine\\CScript.h>

enum class DEFORMOBJ_STATE
{
    IDLE,
    AIR,
    LANDING,
};

class CDeformObjectScript : public CScript
{
private:
    DEFORMOBJ_STATE m_CurState;
    DEFORMOBJ_STATE m_PrevState;
    Vec3 rayCenter;
    float rayDist;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CDeformObjectScript);
    CDeformObjectScript();
    CDeformObjectScript(const CDeformObjectScript& _Origin);
    virtual ~CDeformObjectScript();
};