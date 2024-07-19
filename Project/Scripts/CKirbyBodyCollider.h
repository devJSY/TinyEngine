#pragma once
#include "CUnitScript.h"

class CKirbyBodyCollider : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyBodyCollider)
    CKirbyBodyCollider();
    virtual ~CKirbyBodyCollider();
};