#pragma once
#include <Engine\\CScript.h>

enum class ArrowType
{
    UP,
    DOWN,
};

class CElfilisAirArrow : public CScript
{
private:
    CGameObject* m_Target;
    Vec3 m_InitPos;
    Vec3 m_InitDir[3];
    Vec3 m_ReadyDir;
    ArrowType m_Type;
    UINT m_Step;
    int m_ArrowIdx;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

    void StartSpawn();
    void StartReady();
    void StartAim();

public:
    bool IsSpawnFinished() { return m_Step != 1; }

    void SetTarget(CGameObject* _Target) { m_Target = _Target; }
    void SetInitPos(Vec3 _Pos) { m_InitPos = _Pos; }
    void SetInitDir(Vec3 _Front, Vec3 _Up, Vec3 _Right);
    void SetType(ArrowType _Type) { m_Type = _Type; }
    void SetArrowIdx(int _Idx) { m_ArrowIdx = _Idx; }

private:
    void Spawn();
    void Ready();
    void Aim();
    void Attack();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisAirArrow);
    CElfilisAirArrow();
    virtual ~CElfilisAirArrow();
};
