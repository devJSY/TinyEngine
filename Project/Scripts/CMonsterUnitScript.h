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

    Vec3         m_vDamageDir;
    float        m_fBodyDamage;
    bool         m_IsDamaged;
    bool         m_IsHitPlayerBody;

protected:
    void SetTarget(CGameObject* _pObj) { m_pTargetObj = _pObj; }
    CGameObject* GetTarget() const { return m_pTargetObj; }
    
    virtual void TakeHit(const UnitHit& _info, const bool _IsDamaged, const bool _IsHitPlayerBody, const Vec3 _vDamageDir);
    
    void RigidbodyMove(CGameObject* _pTargetObj = nullptr);
    void TransformRotate();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    virtual CMonsterUnitScript* Clone() = 0;

    CMonsterUnitScript(UINT _Type);
    CMonsterUnitScript(const CMonsterUnitScript& _Origin);
    virtual ~CMonsterUnitScript();

    friend class CHitBoxScript;
    friend class CDetectAreaScript;
};
