#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "components.h"

class CScript : public CComponent
{
private:
    const UINT m_ScriptType;

public:
    UINT GetScriptType() const { return m_ScriptType; }

public:
    virtual void finaltick() final {}

public:
    virtual void SaveToLevelFile(FILE* _File) override = 0;
    virtual void LoadFromLevelFile(FILE* _File) override = 0;

    CScript* Clone() = 0;

public:
    CScript(UINT _ScriptType);
    virtual ~CScript();
};
