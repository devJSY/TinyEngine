#pragma once
#include <Engine\\CScript.h>

class CFXScript : public CScript
{
private:
    float m_Duration;
    float m_PassedTime;

public:
    void SetDuration(float _Duration) { m_Duration = _Duration; };

public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CFXScript)

public:
    CFXScript();
    CFXScript(const CFXScript& origin);
    virtual ~CFXScript();
};
