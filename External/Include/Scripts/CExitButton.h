#pragma once
#include "CButtonScript.h"
class CExitButton 
	: public CButtonScript
{
public:
    virtual void Func() override;

public:
    CExitButton();
    virtual ~CExitButton();
};
