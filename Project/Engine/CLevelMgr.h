#pragma once

class CLevel;

class CLevelMgr : public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);

private:
    CLevel* m_CurLevel;
    std::list<std::thread> m_listLoadThread;
    std::mutex m_Mutex;

public:
    void init();
    void tick();

    void ThreadRelease();

public:
    CLevel* GetCurrentLevel() const { return m_CurLevel; }
    CLevel* CreateDefaultLevel();

public:
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _StartState);
    void ChangeLevelAsync(const wstring& _strPath, LEVEL_STATE _StartState);

private:
    void ChangeLevelAsyncFunc(const wstring& _strPath, LEVEL_STATE _StartState);

public:
    CLevel* CreateDefaultKirbyLevel();

    CLevel* CreateDefaultStartUILevel();
    CLevel* CreateDefaultRobbyUILevel();
    CLevel* CreateDefaultPlayUILevel();
};
