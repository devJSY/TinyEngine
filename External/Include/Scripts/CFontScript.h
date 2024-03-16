#pragma once
#pragma once
#include <Engine\\CScript.h>

class CFontScript : public CScript
{
    string m_FontString;
    Vec2 m_FontPos;
    float m_FontSize;
    Vec4 m_FontColor;

public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CFontScript);

public:
    CFontScript();
    virtual ~CFontScript();
};
