#pragma once
#include <Engine\\CScript.h>

class CGameManagerScript : public CScript
{
private:
    static CGameManagerScript* m_Inst;
    CGameObject* m_Player;

public:
    static CGameManagerScript* GetInset();

public:
    CGameObject* GetPlayer() const { return m_Player; }

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
