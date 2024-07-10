#pragma once

#include "CButtonScript.h"


class CLevelChangeButtonScript 
	: public CButtonScript
{
private:
    string m_LevelName;

public:
    virtual void Func() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLevelChangeButtonScript();
    virtual ~CLevelChangeButtonScript();
};
