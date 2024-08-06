#pragma once
#include <Engine\CScript.h>
class CPushOutTriggerScript : public CScript
{
private:
    CGameObject* m_pParent;

public:
    void SetParent(CGameObject* _pObj) { m_pParent = _pObj; }

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CPushOutTriggerScript)

public:
    CPushOutTriggerScript();
    CPushOutTriggerScript(const CPushOutTriggerScript& Origin);
    virtual ~CPushOutTriggerScript();
};
