#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CScriptMgr.h"

class CScript : public CComponent
{
private:
    SCRIPT_TYPE m_iScriptType;

public:
    virtual void finaltick() final {}

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}

public:
    SCRIPT_TYPE GetScriptType() const { return m_iScriptType; }

public:
    CScript* Clone() = 0;

public:
    CScript(SCRIPT_TYPE _type);
    virtual ~CScript();
};
