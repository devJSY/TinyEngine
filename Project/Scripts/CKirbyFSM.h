#pragma once
#include "CFSMScript.h"
class CKirbyAbility;
class CKirbyObject;
class CKirbyVacuumCollider;
class CKirbyLightScript;

enum class LastJumpType
{
    LOW,
    HIGH,
};

enum class DodgeType
{
    NONE,
    FRONT,
    RIGHT,
    BACK,
    LEFT,
};

class CKirbyFSM : public CFSMScript
{
private:
    CKirbyAbility*          m_arrAbility[(UINT)AbilityCopyType::END];
    AbilityCopyType         m_PrevAbility;
    AbilityCopyType         m_CurAbility;
    AbilityCopyType         m_NextAbility;

    CKirbyObject*           m_arrObject[(UINT)ObjectCopyType::END];
    ObjectCopyType          m_PrevObject;
    ObjectCopyType          m_CurObject;
    ObjectCopyType          m_NextObject;
    
    CGameObject*            m_CurHat;
    CGameObject*            m_CurHatBlade;
    CGameObject*            m_CurWeapon;
    CGameObject*            m_StuffedObj;

    CCapsuleCollider*       m_BodyCollider;
    CKirbyVacuumCollider*   m_VacuumCollider;
    CKirbyLightScript*      m_PointLight;

    // 상태 관리를 위한 값들
    const float             m_HoveringLimitTime;
    float                   m_HoveringAccTime;
    bool                    m_bHovering;

    UINT                    m_ComboLevel;
    float                   m_ComboAccTime;
    float                   m_ChargeAccTime;
    UINT                    m_SlideComboLevel;
    bool                    m_bAttackEvent;

    LastJumpType            m_LastJump;
    DodgeType               m_DodgeType;
    bool                    m_bStuffed;

    Vec3                    m_KnockbackDir;
    float                   m_YPressedTime;
    float                   m_Vacuum1MaxTime;
    float                   m_DropCopyTime;
    bool                    m_bDroppable;
    bool                    m_bNearDeformObject;

    // 무적 상태
    float                   m_InvincibleAcc;
    float                   m_InvincibleDuration;
    bool                    m_bInvincible;

    // Emissive 상태
    float                   m_EmissiveCoef;
    float                   m_EmissiveAcc;
    float                   m_EmissiveDuration;
    bool                    m_bEmissive;
    bool                    m_bIsSkrr;

    bool                    m_bCanBladeAttack;

    // Fire Gliding Timer
    float                   m_GlidingDuration;
    float                   m_GlidingAcc;

    // Vending Machine
    UINT                    m_LeftCanCount;

    // Ladder
    bool                    m_bEscapeLadder;
    bool                    m_bInCollisionLadder;
    Vec3                    m_LadderUpSightDir;
    Vec3                    m_LadderDownSightDir;
    Vec3                    m_LadderTop;
    Vec3                    m_LadderBottom;

public:
    void begin() override;
    void tick() override;

    void ChangeAbilityCopy(AbilityCopyType _Type);
    void ChangeObjectCopy(ObjectCopyType _Type);
    void StartStuffed(CGameObject* _Target);
    void DrawingCollisionEnter(CGameObject* _CollisionObject);
    void AddGlidingTime(float _Time) { m_GlidingAcc += _Time; }

public:
    void SetCurHat(CGameObject* _Hat);
    void SetCurHatBlade(CGameObject* _HatBlade) { m_CurHatBlade = _HatBlade; }
    void SetCurWeapon(CGameObject* _Weapon);
    void SetAttackEvent(bool _bAttack) { m_bAttackEvent = _bAttack; }
    void SetComboLevel(int _Level) { m_ComboAccTime = 0.f; m_ComboLevel = _Level; }
    void SetLastJump(LastJumpType _Type) { m_LastJump = _Type; }
    void SetHovering(bool _bHovering);
    void SetDodgeType(DodgeType _Type) { m_DodgeType = _Type; }
    void SetSlideComboLevel(UINT _bCombo) { m_SlideComboLevel = _bCombo; }
    void SetKnockBackDir(Vec3 _Dir) { m_KnockbackDir = _Dir; }
    void SetInvincible(bool _Invincible, float _Duration = -1.f);
    void SetNearDeformObj(bool _bNearObj) { m_bNearDeformObject = _bNearObj; }
    void SetEmissive(bool _Emissive, float _Duration);
    void SetBladeAttack(bool _CanAttack) { m_bCanBladeAttack = _CanAttack; }
    void SetDroppable(bool _bDroppable) { m_bDroppable = _bDroppable; }
    void SetCollisionLadder(bool _bLadder) { m_bInCollisionLadder = _bLadder; }
    void SetLadderUpSightDir(Vec3 _v) { m_LadderUpSightDir = _v; }
    void SetLadderDownSightDir(Vec3 _v) { m_LadderDownSightDir = _v; }
    void SetLadderTop(Vec3 _Top) { m_LadderTop = _Top; }
    void SetLadderBottom(Vec3 _Bottom) { m_LadderBottom = _Bottom; }
    void SetEscapeLadder(bool _b) { m_bEscapeLadder = _b; }
    void SetSkrr(bool _Skrr) { m_bIsSkrr = _Skrr; }    

    void ClearCurHatWeapon();
    void ClearChargeAccTime() { m_ChargeAccTime = 0.f; }
    void ClearHoveringAccTime() { m_HoveringAccTime = 0.f; }
    void ClearStuff();
    void ClearGlidingTime() { m_GlidingAcc = 0.f; }
    void ClearYPressedTime() { m_YPressedTime = 0.f; }
    void SubCanCount();
    void ResetCanCount() { m_LeftCanCount = 100; }
    void OffCollider();
    void OnCollider();

    CKirbyAbility* GetCurAbility() const { return m_arrAbility[(UINT)m_CurAbility]; }
    CKirbyAbility* GetPrevAbility() const { return m_arrAbility[(UINT)m_PrevAbility]; } 
    CKirbyAbility* GetNextAbility() const { return m_arrAbility[(UINT)m_NextAbility]; } 
    CKirbyObject* GetCurObject() const { return m_arrObject[(UINT)m_CurObject]; }
    CKirbyObject* GetPrevObject() const { return m_arrObject[(UINT)m_PrevObject]; }
    CKirbyObject* GetNextObject() const { return m_arrObject[(UINT)m_NextObject]; }
    AbilityCopyType GetCurAbilityIdx() const { return m_CurAbility; }
    AbilityCopyType GetPrevAbilityIdx() const { return m_PrevAbility; }
    ObjectCopyType GetCurObjectIdx() const { return m_CurObject; }
    ObjectCopyType GetPrevObjectIdx() const { return m_PrevObject; }
    ObjectCopyType GetNextObjectIdx() const { return m_NextObject; }
    CKirbyVacuumCollider* GetVacuumCol() const { return m_VacuumCollider; }
    LastJumpType GetLastJump() const { return m_LastJump; }
    CGameObject* GetCurHat() { return m_CurHat; }
    CGameObject* GetCurHatBlade() { return m_CurHatBlade; }
    CGameObject* GetCurWeapon() { return m_CurWeapon; }
    CGameObject* GetStuffedObj() { return m_StuffedObj; }
    CKirbyLightScript* GetPointLight() { return m_PointLight; }
    DodgeType GetDodgeType() const { return m_DodgeType; }
    float GetComboAccTime() const { return m_ComboAccTime; }
    float GetChargeAccTime() const { return m_ChargeAccTime; }
    float GetHoveringAccTime() const { return m_HoveringAccTime; }
    float GetHoveringLimitTime() const { return m_HoveringLimitTime; }
    UINT GetComboLevel() const { return m_ComboLevel; }
    UINT GetSlideComboLevel() const { return m_SlideComboLevel; }
    float GetGlidingDuration() const { return m_GlidingDuration; }
    float GetCurGlidingTime() const { return m_GlidingAcc; }
    float GetYPressedTime() const { return m_YPressedTime; }
    float GetVacuum1MaxTime() const { return m_Vacuum1MaxTime; }
    float GetDropCopyTime() const { return m_DropCopyTime; }
    bool IsAttackEvent() const { return m_bAttackEvent; }
    bool IsStuffed() const { return m_bStuffed; }
    bool IsHovering() const { return m_bHovering; }
    bool IsDrawing() const;
    bool IsInvincible() const { return m_bInvincible; }
    bool IsNearDeformObject() const { return m_bNearDeformObject; }
    bool CanBladeAttack() const { return m_bCanBladeAttack; }
    Vec3 GetKnockBackDir() const { return m_KnockbackDir; }
    UINT GetCanCount() const { return m_LeftCanCount; }

    bool GetCollisionLadder() const { return m_bInCollisionLadder; }
    Vec3 GetLadderUpSightDir() const { return m_LadderUpSightDir; }
    Vec3 GetLadderDownSightDir() const { return m_LadderDownSightDir; }
    Vec3 GetLadderTop() const { return m_LadderTop; }
    Vec3 GetLadderBottom() const { return m_LadderBottom; }
    bool GetEscapeLadder() const { return m_bEscapeLadder; }


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyFSM)
    CKirbyFSM();
    CKirbyFSM(const CKirbyFSM& _Origin);
    virtual ~CKirbyFSM();

    friend class CKirbyRun;
};
