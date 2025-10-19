#pragma once
#include <Engine/CScript.h>

class CEngineTestScript : public CScript
{
private:
    float TestParam1;
    float TestParam2;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider);
    virtual void OnCollisionExit(CCollider* _OtherCollider);

    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CEngineTestScript);

public:
    CEngineTestScript();
    CEngineTestScript(const CEngineTestScript& origin);
    virtual ~CEngineTestScript();
};
