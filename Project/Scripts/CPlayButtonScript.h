#pragma once
#include "CButtonScript.h"

class CPlayButtonScript : public CButtonScript
{
public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File){};
    virtual void LoadFromLevelFile(FILE* _File){};

    CLONE(CPlayButtonScript);

public:
    CPlayButtonScript();
    virtual ~CPlayButtonScript();
};
