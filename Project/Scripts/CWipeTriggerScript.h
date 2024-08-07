#pragma once
#include <Engine\CScript.h>
class CWipeTriggerScript : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CWipeTriggerScript)
public:
    CWipeTriggerScript();
    CWipeTriggerScript(const CWipeTriggerScript& Origin);
    virtual ~CWipeTriggerScript();
};
