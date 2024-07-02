#pragma once
#include "CFSMScript.h"

enum class ABILITY_COPY_TYPE
{
    NORMAL,
    FIRE,
    RANGER,
    SWORD,

    END,
};

enum class OBJECT_COPY_TYPE
{
    NORMAL,
    CAR,
    STAIR,
    LIGHT,

    END,
};

class CKirbyAbility;
class CKirbyObject;

class CKirbyFSM : public CFSMScript
{
private:
    CKirbyAbility*  m_arrAbility[(UINT)ABILITY_COPY_TYPE::END];
    CKirbyAbility*  m_CurAbility;

    CKirbyObject*   m_arrObject[(UINT)OBJECT_COPY_TYPE::END];
    CKirbyObject*   m_CurObject;

    // 상태 관리를 위한 값들
    float           m_ChargeAccTime;
    UINT            m_bVacuum; // 0: false, 1: true(Lv1), 2: true(Lv2)
    bool            m_bStuffed;

public:
    void begin() override;
    void tick() override;
    void ChangeAbilityCopy(ABILITY_COPY_TYPE _Type);
    void ChangeObjectCopy(OBJECT_COPY_TYPE _Type);

public:
    void SetVacuum(UINT _bVacuum) { m_bVacuum = _bVacuum; }
    void SetStuffed(bool _bStuffed) { m_bStuffed = _bStuffed; }
    void ClearChargeAccTime() { m_ChargeAccTime = 0.f; }

    CKirbyAbility* GetCurAbility() const { return m_CurAbility; }
    CKirbyObject* GetCurObject() const { return m_CurObject; }
    float GetChargeAccTime() const { return m_ChargeAccTime; }
    UINT IsVacuum() const { return m_bVacuum; }
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
