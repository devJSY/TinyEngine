#pragma once
#include <Engine\\CScript.h>

class CConflictTest : public CScript
{
private:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CConflictTest);

public:
    CConflictTest();
    virtual ~CConflictTest();
};