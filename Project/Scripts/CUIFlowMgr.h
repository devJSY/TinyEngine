#pragma once
#include <Engine\CScript.h>

#include "CUIManagerScript.h"

class CUIFlowScript;
class CUIFlowMgr : public CScript
{
private:
    CUIAnimManagerScript* m_pAnimMgrScript;

    CUIFlowScript* m_pFlowScript;
    SceneType m_eType;

public:
    void SetAnimMgrScript(CUIAnimManagerScript* _pScript) { m_pAnimMgrScript = _pScript; }

    void SetSceneType(const SceneType _type) { m_eType = _type; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE_DISABLE(CUIFlowMgr)
public:
    CUIFlowMgr();
    virtual ~CUIFlowMgr();
};
