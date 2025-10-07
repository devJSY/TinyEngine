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

    void ThreadRelease();

public:
    CLevel* GetCurrentLevel() const { return m_CurLevel; }
    CLevel* CreateDefaultLevel();

public:
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _StartState);
};
