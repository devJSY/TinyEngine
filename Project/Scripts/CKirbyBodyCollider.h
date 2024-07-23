#pragma once
#include "CUnitScript.h"

class CKirbyBodyCollider : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyBodyCollider)
    CKirbyBodyCollider();
    virtual ~CKirbyBodyCollider();
};