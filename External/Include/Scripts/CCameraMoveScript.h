#pragma once
#include <Engine\\CScript.h>

class CCameraMoveScript : public CScript
{
private:
    float m_CamSpeed;
    bool m_bFocus;

public:
    virtual void tick();

private:
    void MoveOrthographic();
    void MovePerspective();

private:
    void MoveFocusOrthographic();
    void MoveFocusPerspective();

    CLONE(CCameraMoveScript);

public:
    CCameraMoveScript();
    virtual ~CCameraMoveScript();
};
