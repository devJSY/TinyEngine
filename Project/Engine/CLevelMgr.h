#pragma once

class CLevel;

class CLevelMgr : public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);

private:
    CLevel* m_CurLevel;

public:
    void init();
    void tick();

public:
    CLevel* GetCurrentLevel() const { return m_CurLevel; }
    CLevel* CreateNewLevel();

public:
    CLevel* CreateDefaultKirbyLevel();

    // DefaultUILevel
    void CreateDefaultUILevel();

    // DefaultUISubLevel
    CLevel* CreateDefaultStartUILevel();
    CLevel* CreateDefaultRobbyUILevel();
    CLevel* CreateDefaultPlayUILevel();

public:
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _StartState);
};
