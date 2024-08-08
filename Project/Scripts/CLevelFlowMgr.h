#pragma once

#include <Engine/CScript.h>

class CLevelFlowMgr : public CScript
{
private:
    string m_NextLevelPath;

    CLevel* m_CurLevel;
    CLevel* m_NextLevel;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void LevelInit();
    virtual void LevelExit();
    virtual void LevelRestart();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    virtual CLevelFlowMgr* Clone() = 0;
    CLevelFlowMgr(UINT _Type);
    CLevelFlowMgr(const CLevelFlowMgr& _Origin);
    virtual ~CLevelFlowMgr();

};
