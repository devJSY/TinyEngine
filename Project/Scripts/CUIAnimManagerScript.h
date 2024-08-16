#pragma once
#include <Engine/CScript.h>

class CUIAnimScript;
class CUIAnimManagerScript : public CScript
{
private:
    vector<CUIAnimScript*> m_vAnimScript;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void EndAnimUI();
    bool AllFinishEndUI();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE_DISABLE(CUIAnimManagerScript)
public:
    CUIAnimManagerScript();
    virtual ~CUIAnimManagerScript();
};
