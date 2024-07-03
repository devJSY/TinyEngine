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
    NONE,
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

enum class JumpType
{
    LOW,
    HIGH,
};

class CKirbyAbility;
class CKirbyObject;

class CKirbyFSM : public CFSMScript
{
private:
    CKirbyAbility*  m_arrAbility[(UINT)AbilityCopyType::END];
    AbilityCopyType m_CurAbility;

    CKirbyObject*   m_arrObject[(UINT)ObjectCopyType::END];
    ObjectCopyType  m_CurObject;

    // 상태 관리를 위한 값들
    float           m_ChargeAccTime;
    ChargeType      m_Charge;
    JumpType        m_LastJump;
    bool            m_bStuffed;

public:
    void begin() override;
    void tick() override;
    void ChangeAbilityCopy(AbilityCopyType _Type);
    void ChangeObjectCopy(ObjectCopyType _Type);

public:
    void SetCharge(ChargeType _Charge) { m_Charge = _Charge; }
    void SetLastJump(JumpType _Type) { m_LastJump = _Type; }
    void SetStuffed(bool _bStuffed) { m_bStuffed = _bStuffed; }
    void ClearChargeAccTime() { m_ChargeAccTime = 0.f; }

    CKirbyAbility* GetCurAbility() const { return m_arrAbility[(UINT)m_CurAbility]; }
    CKirbyObject* GetCurObject() const { return m_arrObject[(UINT)m_CurObject]; }
    AbilityCopyType GetCurAbilityIdx() const { return m_CurAbility; }
    ObjectCopyType GetCurObjectIdx() const { return m_CurObject; }
    JumpType GetLastJump() const { return m_LastJump; }
    ChargeType IsCharge() const { return m_Charge; }
    float GetChargeAccTime() const { return m_ChargeAccTime; }
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
