#pragma once
#include "CFSMScript.h"

enum class AbilityCopyType
{
    NORMAL,
    FIRE,
    RANGER,
    SWORD,

    END,
};

enum class ObjectCopyType
{
    NORMAL,
    CAR,
    STAIR,
    LIGHT,

    END,
};

enum class ChargeType
{
    NONE,
    LV1,
    LV2,
};

class CKirbyAbility;
class CKirbyObject;

class CKirbyFSM : public CFSMScript
{
private:
    CKirbyAbility*  m_arrAbility[(UINT)AbilityCopyType::END];
    CKirbyAbility*  m_CurAbility;

    CKirbyObject*   m_arrObject[(UINT)ObjectCopyType::END];
    CKirbyObject*   m_CurObject;

    // 상태 관리를 위한 값들
    float           m_ChargeAccTime;
    ChargeType      m_bCharge;
    bool            m_bStuffed;

public:
    void begin() override;
    void tick() override;
    void ChangeAbilityCopy(AbilityCopyType _Type);
    void ChangeObjectCopy(ObjectCopyType _Type);

public:
    void SetCharge(ChargeType _bCharge) { m_bCharge = _bCharge; }
    void SetStuffed(bool _bStuffed) { m_bStuffed = _bStuffed; }
    void ClearChargeAccTime() { m_ChargeAccTime = 0.f; }

    CKirbyAbility* GetCurAbility() const { return m_CurAbility; }
    CKirbyObject* GetCurObject() const { return m_CurObject; }
    float GetChargeAccTime() const { return m_ChargeAccTime; }
    ChargeType IsCharge() const { return m_bCharge; }
    bool IsStuffed() const { return m_bStuffed; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyFSM)
    CKirbyFSM();
    CKirbyFSM(const CKirbyFSM& _Origin);
    virtual ~CKirbyFSM();

    friend class CKirbyRun;
};
