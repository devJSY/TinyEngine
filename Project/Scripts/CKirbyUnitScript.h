#pragma once
#include "CUnitScript.h"

class CKirbyUnitScript : public CUnitScript
{
public:
    CLONE(CKirbyUnitScript)
    CKirbyUnitScript();
    CKirbyUnitScript(const CKirbyUnitScript& _Origin);
    virtual ~CKirbyUnitScript();
};
