#pragma once
#include "CFSMScript.h"
class CElfilisBigFSM;

enum class ElfilisStateGroup
{
    DEMO,

    GroundIdle,
    GroundMove,
    GroundMoveAtk,
    GroundAtkNear,
    GroundAtkFar,
    GroundToAir,

    AirIdle,
    AirMove,
    AirSmallAtk1,
    AirSmallAtk2,
    AirLargeAtk,
    AirToGround,

    END,
};

enum class ElfilisPatternType
{
    NONE,

    // demo
    Appear1,
    Appear2,

    // combo
    BigCombo,
    StabCombo,
};

class CElfilisFSM : public CFSMScript
{
private:
    // FSM
    map<ElfilisStateGroup, vector<wstring>[2]> m_StateGroup; // ( StateGroup, {{PublicStates}, {PrivateStates}} )
    map<wstring, UINT> m_StateSelectionCount[(int)ElfilisStateGroup::END];
    ElfilisStateGroup m_CurStateGroup;
    ElfilisPatternType m_Pattern;
    UINT m_Phase;
    UINT m_ComboLevel;
    UINT m_PatternStep;
    bool m_bAttackRepeat;
    bool m_bResist;

    // ground
    UINT m_GroundAttackCount;
    float m_NearDist;

    // air
    Vec3 m_AirPosition;
    bool m_bGroundCollision;

    // big Elfilis
    CGameObject* m_BigElfilis;
    CElfilisBigFSM* m_BigElfilisFSM;

    // childs
    CGameObject* m_Weapon;
    CBoxCollider* m_Hitbox;

    // materials
    vector<Ptr<CMaterial>> m_listBodyMtrl;
    vector<Ptr<CMaterial>> m_listWeaponMtrl;
    vector<Ptr<CTexture>> m_listBodyEmissiveTex;
    vector<Ptr<CTexture>> m_listWeaponEmissiveTex;
    vector<Vec3> m_listBodyEmissive;
    vector<Vec3> m_listWeaponEmissive;

    // map
    Vec3 m_MapFloorOffset;
    float m_MapSizeRadius;
    Ptr<CPrefab> m_DropStarPref;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void OnCollisionEnter(CCollider* _OtherCollider) override;

    void ResetFSM();
    void ChangeStateGroup(ElfilisStateGroup _Group, const wstring& _State = L"");
    void RepeatState(wstring _State = L"");
    ElfilisStateGroup FindNextStateGroup() const;

    void OnWeaponTrigger();
    void OffWeaponTrigger();
    void ResetEmissive();

    void SpawnDropStar(Vec3 _Pos);
    void DestroySumon();

public:
    void ClearComboLevel() { m_ComboLevel = 0; }
    void AddComboLevel() { m_ComboLevel++; }
    void SetPattern(ElfilisPatternType _Pattern);
    void ProcPatternStep();
    void SetPhase(int _Phase) { m_Phase = _Phase; }
    void SetResist(bool _bResist) { m_bResist = _bResist; }
    void AddEmissive(Vec3 _Color);

    ElfilisStateGroup GetCurStateGroup() const { return m_CurStateGroup; }
    const vector<wstring>& GetCurPublicStates() const;
    bool IsPattern() const { return m_Pattern != ElfilisPatternType::NONE; }
    bool IsPattern(ElfilisPatternType _Pattern, UINT _Step) const { return m_Pattern == _Pattern && m_PatternStep == _Step; }
    UINT GetPhase() const { return m_Phase; }
    bool IsResist() { return m_bResist; }
    UINT GetComboLevel() const { return m_ComboLevel; }
    UINT GetPatternStep() const { return m_PatternStep; }
    float GetNearDist() const { return m_NearDist; }
    bool IsNearPlayer() const { return GetPlayerDist() <= m_NearDist; }
    Vec3 GetAirPos() const { return m_AirPosition; }
    bool IsGround() const { return m_bGroundCollision; }
    CGameObject* GetBigBoss() const { return m_BigElfilis; }
    CElfilisBigFSM* GetBigBossFSM() const { return m_BigElfilisFSM; }
    CGameObject* GetWeapon() const { return m_Weapon; }
    CBoxCollider* GetHitbox() const { return m_Hitbox; }
    float GetMapSizeRadius() const { return m_MapSizeRadius; }
    Vec3 GetMapFloorOffset() const { return m_MapFloorOffset; }
    float GetPlayerDist() const;

private:
    void ChangeStateGroup_Random(ElfilisStateGroup _Group);
    void ChangeStateGroup_Set(ElfilisStateGroup _Group, const wstring& _State);
    void SetStateGroup(ElfilisStateGroup _Group);
    void AddGroupPublicState(ElfilisStateGroup _Group, const wstring& _StateName, CState* _State);
    void AddGroupPrivateState(ElfilisStateGroup _Group, const wstring& _StateName, CState* _State);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CElfilisFSM)
    CElfilisFSM();
    CElfilisFSM(const CElfilisFSM& _Origin);
    virtual ~CElfilisFSM();
};