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
    CKirbyAbility* m_arrAbility[(UINT)ABILITY_COPY_TYPE::END];
    CKirbyAbility* m_CurAbility;

    CKirbyObject* m_arrObject[(UINT)OBJECT_COPY_TYPE::END];
    CKirbyObject* m_CurObject;

    // 상태 관리를 위한 값들
    bool m_bVacuum;

public:
    void begin() override;

public:
    void ChangeAbilityCopy(ABILITY_COPY_TYPE _Type);
    void ChangeObjectCopy(OBJECT_COPY_TYPE _Type);

    void SetVacuum(bool _bVacuum) { m_bVacuum = _bVacuum; }

    CKirbyAbility* GetCurAbility() const { return m_CurAbility; }
    CKirbyObject* GetCurObject() const { return m_CurObject; }
    bool GetVacuum() const { m_bVacuum; }

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
