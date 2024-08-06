#pragma once
#include <Engine\\CScript.h>

class ElfilisDimensionSpear;

class ElfilisDimensionSet : public CScript
{
private:
    CGameObject* m_Spear[5];
    ElfilisDimensionSpear* m_SpearScript[5];
    Vec3 m_InitPos;
    Vec3 m_ArrDir;
    UINT m_Step;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitPos(Vec3 _Pos) { m_InitPos = _Pos; }
    void SetArrDir(Vec3 _Dir) { m_ArrDir = _Dir; }

private:
    void Open();
    void Ready();
    void Attack();
    void AttackEnd();
    void Close();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(ElfilisDimensionSet);
    ElfilisDimensionSet();
    virtual ~ElfilisDimensionSet();
};
