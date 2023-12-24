#pragma once
#include "CScript.h"
class CCameraMoveScript : public CScript
{
private:


public:
    virtual void tick();

private:
    void MoveOrthographic();
    void MovePerspective();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};
