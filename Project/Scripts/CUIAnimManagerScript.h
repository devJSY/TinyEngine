#pragma once
#include <Engine/CScript.h>
#include "CUIManagerScript.h"

class CUIFlowScript;
class CUIAnimManagerScript : public CScript
{
private:
    vector<CUIFlowScript*> m_vFlowScript;
    SceneType m_eType;

public:
    void SetSceneType(const SceneType _type) { m_eType = _type; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE_DISABLE(CUIAnimManagerScript)
public:
    CUIAnimManagerScript();
    virtual ~CUIAnimManagerScript();
};
