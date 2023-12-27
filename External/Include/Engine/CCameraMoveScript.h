#pragma once
#include "CScript.h"
class CCameraMoveScript : public CScript
{
private:
    bool m_bFocus;

public:
    virtual void tick();

private:
    void MoveOrthographic();
    void MovePerspective();

private:
    void MoveFocusOrthographic();
    void MoveFocusPerspective();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};
