#pragma once
#include "CUnitScript.h"

enum class PATROLDIR
{
    Up,
    Down,
    Right,
    Left,

    UpLeft,
    UpRight,
    DownLeft,
    DownRight,

    END,
};

class CMonsterUnitScript : public CUnitScript
{
private:
    CGameObject* m_pTargetObj;
    float m_RaycastDist;
    UnitHit m_HitInfo;

protected:
    void SetTarget(CGameObject* _pObj) { m_pTargetObj = _pObj; }
    CGameObject* GetTarget() const { return m_pTargetObj; }

    const UnitHit& GetHitInfo() { return m_HitInfo; }
    void SetHitInfo(const UnitHit& _hitInfo) { m_HitInfo = _hitInfo; }

    void RigidbodyMove(CGameObject* _pTargetObj = nullptr);
    void Rotating();
    void RotatingToTarget();
    bool IsGround();


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    virtual CMonsterUnitScript* Clone() = 0;

    CMonsterUnitScript(UINT _Type);
    CMonsterUnitScript(const CMonsterUnitScript& _Origin);
    virtual ~CMonsterUnitScript();

    friend class CDetectAreaScript;
};
