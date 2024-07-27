#pragma once
#include <Engine\\CScript.h>

class CUnitScript;
class CFSMScript;

class CBossMgr : public CScript
{
public:
    wstring m_BossName;

private:
    static CGameObject* m_Boss;
    static CUnitScript* m_BossUnit;
    static CFSMScript* m_BossFSM;

public:
    virtual void begin() override;

public:
    void SetBoss(CGameObject* _BossObj);

    static CGameObject* GetBoss() { return m_Boss; }
    static CUnitScript* GetBossScript() { return m_BossUnit; }
    static CFSMScript* GetBossFSM() { return m_BossFSM; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CBossMgr);
    CBossMgr();
    virtual ~CBossMgr();
};
