#pragma once
#include "CButtonScript.h"

class CPlayButtonScript : public CButtonScript
{
public:
    virtual void tick() override;

public:
    CPlayButtonScript();
    virtual ~CPlayButtonScript();
};
