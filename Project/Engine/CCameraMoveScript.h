#pragma once
#include "CScript.h"
class CCameraMoveScript : public CScript
{
private:
public:
    virtual void tick();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};
