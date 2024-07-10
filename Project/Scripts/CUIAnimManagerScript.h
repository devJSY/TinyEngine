#pragma once
#include <Engine/CScript.h>


class CUIAnimScript;
class CUIAnimManagerScript 
	: public CScript
{
private:
    vector<CUIAnimScript*> m_vUIAnimObject;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE_DISABLE(CUIAnimManagerScript)
public:
    CUIAnimManagerScript();
    virtual ~CUIAnimManagerScript();
};
