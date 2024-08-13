#pragma once
#include <Engine/CScript.h>
class CLevelFlowMgr;

class CFlowTriggerScript : public CScript
{
private:
    CLevelFlowMgr* m_LevelFlowMgr;
    int m_TriggerEnterEvent;
    int m_TriggerExitEvent;
    bool m_bDestroyFlag[2];

public:
    virtual void begin() override;
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    CLONE(CFlowTriggerScript);
    CFlowTriggerScript();
    CFlowTriggerScript(const CFlowTriggerScript& _Origin);
    virtual ~CFlowTriggerScript();
};