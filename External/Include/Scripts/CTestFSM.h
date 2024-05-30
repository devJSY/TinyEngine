#pragma once
#include "CFSMScript.h"
class CTestFSM :
    public CFSMScript
{
public:
    virtual void begin() override;

    CLONE(CTestFSM);
public:
    CTestFSM();
    virtual ~CTestFSM();
};

