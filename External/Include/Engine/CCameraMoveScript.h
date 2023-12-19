#pragma once
#include "CScript.h"
class CCameraMoveScript : public CScript
{
private:
    float m_CamSpeed;

public:
    virtual void tick();

private:
    void MoveOrthographic();
    void MovePerspective();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};
