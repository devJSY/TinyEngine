#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "components.h"
#include "CPhysicsMgr.h"

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    OBJECT,
    STRING,
};

struct tScriptParam
{
    SCRIPT_PARAM eParam;
    void* pData;
    string strDesc;
    float DragSpeed;
};

class CScript : public CComponent
{
private:
    const UINT m_ScriptType;
    vector<tScriptParam> m_vecParam;

public:
    UINT GetScriptType() const { return m_ScriptType; }
    const vector<tScriptParam>& GetScritpParam() { return m_vecParam; }

protected:
    void AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc, float DragSpeed = 1.f)
    {
        m_vecParam.push_back(tScriptParam{eParam, _pData, _Desc, DragSpeed});
    }

public:
    virtual void finaltick() final {}

private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider){};
    virtual void OnCollisionStay(CCollider2D* _OtherCollider){};
    virtual void OnCollisionExit(CCollider2D* _OtherCollider){};

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider){};
    virtual void OnTriggerStay(CCollider2D* _OtherCollider){};
    virtual void OnTriggerExit(CCollider2D* _OtherCollider){};

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider){};
    virtual void OnCollisionStay(CCollider* _OtherCollider){};
    virtual void OnCollisionExit(CCollider* _OtherCollider){};

    virtual void OnTriggerEnter(CCollider* _OtherCollider){};
    virtual void OnTriggerStay(CCollider* _OtherCollider){};
    virtual void OnTriggerExit(CCollider* _OtherCollider){};

    virtual void OnControllerColliderHit(struct ControllerColliderHit Hit){};

public:
    virtual void SaveToLevelFile(FILE* _File) override = 0;
    virtual void LoadFromLevelFile(FILE* _File) override = 0;

    virtual CScript* Clone() = 0;

public:
    CScript(UINT _ScriptType);
    CScript(const CScript& origin);
    virtual ~CScript();

    friend class CCollider2D;
    friend class CCollider;
    friend class CCharacterController;
};
