#pragma once
#include <Engine\\CScript.h>

class CButtonScript : public CScript
{
protected:
    bool m_bClicked;

public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CButtonScript);

public:
    CButtonScript(SCRIPT_TYPE _Type = BUTTONSCRIPT);
    virtual ~CButtonScript();
};
