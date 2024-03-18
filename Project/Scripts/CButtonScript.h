#pragma once
#include <Engine\\CScript.h>

class CButtonScript : public CScript
{
protected:
    bool m_bClicked;

public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;

    CButtonScript* Clone() = 0;

public:
    CButtonScript(SCRIPT_TYPE _Type);
    virtual ~CButtonScript();
};
