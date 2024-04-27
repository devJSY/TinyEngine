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

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCameraMoveScript);

public:
    CCameraMoveScript();
    CCameraMoveScript(const CCameraMoveScript& origin);
    virtual ~CCameraMoveScript();
};
