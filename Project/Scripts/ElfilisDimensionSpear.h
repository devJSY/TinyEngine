#pragma once
#include <Engine\\CScript.h>

class ElfilisDimensionSpear : public CScript
{
private:
    Ptr<CPrefab> SpikeEffect;
    Ptr<CPrefab> RockEffect;
    Vec3 m_InitPos;
    Vec3 m_InitRot;
    UINT m_Step;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitPos(Vec3 _Pos) { m_InitPos = _Pos; }
    void SetInitRot(Vec3 _Rot) { m_InitRot = _Rot; }

private:
    void Spawn();
    void AttackReday();
    void Attack();
    void AttackEnd();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(ElfilisDimensionSpear);
    ElfilisDimensionSpear();
    virtual ~ElfilisDimensionSpear();
};