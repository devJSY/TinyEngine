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
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLevelChangeButtonScript();
    virtual ~CLevelChangeButtonScript();
};
