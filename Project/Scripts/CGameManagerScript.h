#pragma once
#include <Engine\\CScript.h>

class CGameManagerScript : public CScript
{
private:
    static CGameManagerScript* m_Inst;
    CGameObject* m_Player;
    CGameObject* m_ExitElevator;

public:
    static CGameManagerScript* GetInset();

private:
    void Release();

public:
    CGameObject* GetPlayer() const { return m_Player; }
    CGameObject* GetExitElevator();
    void ChangeLevel(const std::string& _LevelName);

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE_DISABLE(CGameManagerScript);

public:
    CGameManagerScript();
    virtual ~CGameManagerScript();
};
