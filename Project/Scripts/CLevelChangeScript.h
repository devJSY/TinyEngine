#pragma once
#include <Engine\\CScript.h>

class CLevelChangeScript : public CScript
{
private:
    string m_ChangeLevelName;

public:
    virtual void tick() override;

public:
    void ChangeLevel(const std::string& LevelName);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CLevelChangeScript);

public:
    CLevelChangeScript();
    virtual ~CLevelChangeScript();
};
