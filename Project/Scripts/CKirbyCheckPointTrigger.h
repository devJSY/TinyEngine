#pragma once
#include <Engine/CScript.h>

// Sclae Absolute 사용(Transform)
// Transform y축 회전만 사용 할 것
// Box Collider
// Collider의 center, scale 건들지 x

class CKirbyCheckPointTrigger : public CScript
{
private:
    Vec3 m_Offset;

public:
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    CLONE(CKirbyCheckPointTrigger);
    CKirbyCheckPointTrigger();
    CKirbyCheckPointTrigger(const CKirbyCheckPointTrigger& _Origin);
    virtual ~CKirbyCheckPointTrigger();

};
