#pragma once
#include "CUnitScript.h"

class CMonsterUnitScript : public CUnitScript
{
private:
    CGameObject* m_pTargetObject;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CMonsterUnitScript)

    CMonsterUnitScript();
    CMonsterUnitScript(SCRIPT_TYPE _type);
    CMonsterUnitScript(const CMonsterUnitScript& _Origin);
    virtual ~CMonsterUnitScript();

protected:
    void SetTarget(CGameObject* _pObj) { m_pTargetObject = _pObj; }
    CGameObject* GetTarget() { return m_pTargetObject; }

    friend class CPlayerDetectScript;
    friend class CHitboxScript;
};
