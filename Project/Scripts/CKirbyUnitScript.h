#pragma once
#include "CUnitScript.h"

class CKirbyUnitScript : public CUnitScript
{
public:
    virtual void begin();

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _File);

public:
    CLONE(CKirbyUnitScript)
    CKirbyUnitScript();
    CKirbyUnitScript(const CKirbyUnitScript& _Origin);
    virtual ~CKirbyUnitScript();
};
