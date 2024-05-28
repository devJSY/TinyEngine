#pragma once
#include <Engine\\CScript.h>

class CPhysics2DScript : public CScript
{
public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPhysics2DScript);

public:
    CPhysics2DScript();
    virtual ~CPhysics2DScript();
};
