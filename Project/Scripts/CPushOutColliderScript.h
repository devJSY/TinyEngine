#pragma once
#include <Engine\CScript.h>
class CPushOutColliderScript : public CScript
{
private:
    CGameObject* m_pParent;

public:
    void SetParent(CGameObject* _pObj) { m_pParent = _pObj; }

private:
    virtual void OnTriggerStay(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CPushOutColliderScript)

public:
    CPushOutColliderScript();
    CPushOutColliderScript(const CPushOutColliderScript& Origin);
    virtual ~CPushOutColliderScript();
};
