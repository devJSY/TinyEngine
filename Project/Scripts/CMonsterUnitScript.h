#pragma once
#include "CUnitScript.h"

class CMonsterUnitScript : public CUnitScript
{
private:
    CGameObject* m_pTargetObj;
    float m_RaycastDist;
    bool m_bEatable;

public:
    void SetEatable(bool _bEatable) { m_bEatable = _bEatable; }
    bool IsEatable() const { return m_bEatable; }

protected:
    void SetTarget(CGameObject* _pObj) { m_pTargetObj = _pObj; }
    CGameObject* GetTarget() const { return m_pTargetObj; }

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
