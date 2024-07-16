#pragma once
#include "CUnitScript.h"

class CMonsterUnitScript : public CUnitScript
{
private:
    CGameObject* m_pTargetObj;

protected:
    void SetTarget(CGameObject* _pObj) { m_pTargetObj = _pObj; }
    CGameObject* GetTarget() const { return m_pTargetObj; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    virtual CMonsterUnitScript* Clone() = 0;

    CMonsterUnitScript(UINT _Type);
    CMonsterUnitScript(const CMonsterUnitScript& _Origin);
    virtual ~CMonsterUnitScript();

    friend class CHitBoxScript;
    friend class CDetectAreaScript;
};
