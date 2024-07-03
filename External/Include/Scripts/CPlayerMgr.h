#pragma once
#include <Engine\\CScript.h>

class CUnitScript;
class CKirbyFSM;
class CKirbyMoveController;

class CPlayerMgr : public CScript

{
private:
    static CGameObject*         m_PlayerObj;
    static CUnitScript*         m_PlayerUnit;
    static CKirbyFSM*           m_PlayerFSM;
    static CKirbyMoveController* m_PlayerController;

public:
    virtual void begin() override;
    virtual void tick() override;

    // void SavePlayerInfo();

public:
    void SetPlayer(CGameObject* _PlayerObj);

    static CGameObject* GetPlayer() { return m_PlayerObj; }
    static CUnitScript* GetPlayerScript() { return m_PlayerUnit; }
    static CKirbyFSM* GetPlayerFSM() { return m_PlayerFSM; }
    static CKirbyMoveController* GetPlayerCtrl() { return m_PlayerController; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CPlayerMgr);

public:
    CPlayerMgr();
    virtual ~CPlayerMgr();
};