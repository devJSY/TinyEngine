#pragma once
#include "CLevel.h"

class CTestLevel2 : public CLevel
{
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

public:
    CTestLevel2();
    virtual ~CTestLevel2();
};
