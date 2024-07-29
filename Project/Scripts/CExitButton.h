#pragma once
#include "CButtonScript.h"
class CExitButton 
	: public CButtonScript
{
public:
    virtual void Func() override;

    CLONE(CExitButton)
public:
    CExitButton();
    CExitButton(const CExitButton& origin);
    virtual ~CExitButton();
};
