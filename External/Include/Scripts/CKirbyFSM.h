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

class CKirbyFSM : public CFSMScript
{
private:
    CKirbyAbility* m_arrAbility[(UINT)ABILITY_COPY_TYPE::END];
    CKirbyAbility* m_CurAbility;

    CScript* m_arrObject[(UINT)OBJECT_COPY_TYPE::END];
    CScript* m_CurObject;

public:
    void begin() override;

public:
    void ChangeAbilityCopy(ABILITY_COPY_TYPE _Type);
    void ChangeObjectCopy(OBJECT_COPY_TYPE _Type);

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
