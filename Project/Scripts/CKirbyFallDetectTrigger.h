#pragma once
#include <Engine/CScript.h>
class CKirbyFallDetectTrigger : public CScript
{
public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    CLONE(CKirbyFallDetectTrigger);
    CKirbyFallDetectTrigger();
    CKirbyFallDetectTrigger(const CKirbyFallDetectTrigger& _Origin);
    virtual ~CKirbyFallDetectTrigger();
};
