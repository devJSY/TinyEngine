#pragma once
#include "CUnitScript.h"

class CMonsterUnitScript : public CUnitScript
{
private:
    CGameObject* m_pTargetObj;
    float m_RaycastDist;
    bool m_bEatable;

    bool m_bSparkleOnOff;
    bool m_bSparkle;
    float m_fTermTime;
    float m_fAccTime;

public:
    void SetEatable(bool _bEatable) { m_bEatable = _bEatable; }
    bool IsEatable() const { return m_bEatable; }

    void SetSparkle(bool _bFlag) { 
        m_fAccTime = 0.f;
        m_bSparkle = _bFlag;
    }

public:
    virtual void tick() override;

protected:
    void SetTarget(CGameObject* _pObj) { m_pTargetObj = _pObj; }
    CGameObject* GetTarget() const { return m_pTargetObj; }

    void RigidbodyMove(CGameObject* _pTargetObj = nullptr);
    void Rotating();
    void RotatingToTarget();
    bool IsGround();

    void SparkleEffect();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    virtual CMonsterUnitScript* Clone() = 0;

    CMonsterUnitScript(UINT _Type);
    CMonsterUnitScript(const CMonsterUnitScript& _Origin);
    virtual ~CMonsterUnitScript();

    friend class CDetectAreaScript;
};
