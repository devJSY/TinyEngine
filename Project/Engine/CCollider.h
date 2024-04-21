#pragma once
#include "CComponent.h"

class CCollider : public CComponent
{
protected:
    bool m_bTrigger;
    Ptr<CPhysicMaterial> m_Mtrl;
    Vec3 m_Center;

    int m_CollisionCount;
    int m_TriggerCount;

public:
    virtual void finaltick() override;

public:
    bool IsTrigger() const { return m_bTrigger; }
    void SetTrigger(bool _trigger) { m_bTrigger = _trigger; };

    Ptr<CPhysicMaterial> GetMaterial() const { return m_Mtrl; }
    void SetMaterial(Ptr<CPhysicMaterial> _Mtrl) { m_Mtrl = _Mtrl; };

    Vec3 GetCenter() const { return m_Center; }
    void SetCenter(Vec3 _Center) { m_Center = m_Center; };

private:
    void OnCollisionEnter(CCollider* _OtherCollider);
    void OnCollisionStay(CCollider* _OtherCollider);
    void OnCollisionExit(CCollider* _OtherCollider);

    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerStay(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual CCollider* Clone() = 0;

public:
    CCollider(COMPONENT_TYPE _Type);
    CCollider(const CCollider& origin);
    virtual ~CCollider();

    friend class CPhysicMgr;
};
