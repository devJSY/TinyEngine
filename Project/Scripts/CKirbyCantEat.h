#pragma once
#include "CUnitScript.h"

class CKirbyCantEat : public CScript
{
public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyCantEat)
    CKirbyCantEat();
    virtual ~CKirbyCantEat();
};
