#pragma once
#include <Engine\CScript.h>
class CMovingObjTriggerScript : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMovingObjTriggerScript)

public:
    CMovingObjTriggerScript();
    CMovingObjTriggerScript(const CMovingObjTriggerScript& Origin);
    virtual ~CMovingObjTriggerScript();
};
