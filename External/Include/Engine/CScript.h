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

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

public:
    virtual void SaveToLevelFile(FILE* _File){};
    virtual void LoadFromLevelFile(FILE* _File){};

public:
    CScript(UINT _ScriptType);
    virtual ~CScript();
};
