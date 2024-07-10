#pragma once
#include <Engine\\CScript.h>

class CAnimationTestScript : public CScript
{
public:
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider);
    virtual void OnCollisionExit(CCollider* _OtherCollider);

    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};
    CLONE(CAnimationTestScript);

public:
    CAnimationTestScript();
    virtual ~CAnimationTestScript();
};
