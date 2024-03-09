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

private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider){};
    virtual void OnCollisionStay(CCollider2D* _OtherCollider){};
    virtual void OnCollisionExit(CCollider2D* _OtherCollider){};

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider){};
    virtual void OnTriggerStay(CCollider2D* _OtherCollider){};
    virtual void OnTriggerExit(CCollider2D* _OtherCollider){};

public:
    virtual void SaveToLevelFile(FILE* _File) override = 0;
    virtual void LoadFromLevelFile(FILE* _File) override = 0;

    CScript* Clone() = 0;

public:
    CScript(UINT _ScriptType);
    virtual ~CScript();

    friend class CCollider2D;
};
