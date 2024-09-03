#pragma once
#include <Engine/CScript.h>

class CKirbyObjDetectCollider : public CScript
{
private:
    bool m_bNearObj;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    virtual void tick() override;

private:
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CKirbyObjDetectCollider);
    CKirbyObjDetectCollider();
    virtual ~CKirbyObjDetectCollider();
};