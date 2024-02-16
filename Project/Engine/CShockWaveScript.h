#pragma once
#include "CScript.h"

class CShockWaveScript : public CScript
{
private:
    float m_Time;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    CLONE(CShockWaveScript);

public:
    CShockWaveScript();
    virtual ~CShockWaveScript();
};
