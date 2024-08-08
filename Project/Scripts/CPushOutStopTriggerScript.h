#pragma once

#include <Engine\CScript.h>

class CPushOutStopTriggerScript : public CScript
{
private:
    bool m_bReturn;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CPushOutStopTriggerScript)
public:
    CPushOutStopTriggerScript();
    CPushOutStopTriggerScript(const CPushOutStopTriggerScript& Origin);
    virtual ~CPushOutStopTriggerScript();
};
