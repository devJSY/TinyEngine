#pragma once
#include "CFSMScript.h"

enum class MorphoStateGroup
{
    DEMO,

    Idle,

    MoveToGround,
    MoveToAir,

    AtkGroundWait,
    AtkGroundNormalNear,
    AtkGroundNormalFar,
    AtkGroundTeleport1,
    AtkGroundTeleport2,
    AtkAir1,
    AtkAir2,

    END,
};

enum class MorphoPatternType
{
    NONE,
    Demo_TeleportDoubleSword,

    DoubleSword,
    TeleportCombo,
    TrackingSoulCombo,
    ShockWaveCombo,
};

class CMorphoFSM : public CFSMScript
{
private:
    // FSM
    map<MorphoStateGroup, vector<wstring>[2]> m_StateGroup;  // ( StateGroup, States )
    MorphoStateGroup m_CurStateGroup;
    MorphoPatternType m_Pattern;
    UINT m_Phase;
    UINT m_PatternStep;
    UINT m_ComboLevel;
    float m_NearDist;
    bool m_bAttackRepeat;

    // Child
    CGameObject* m_WeaponL;
    CGameObject* m_WeaponR;
    vector<CGameObject*> m_vecShockWave;

    // materials
    list<Ptr<CMaterial>> m_listBodyMtrl;
    list<Ptr<CMaterial>> m_listWeaponMtrl;
    float m_TeleportAppearTime;
    float m_EmissiveTime;

    // map
    Vec3 m_MapFloorOffset;
    Vec3 m_MapSize;

public:
    virtual void begin() override;
    virtual void tick() override;

    void Move();
    void Attack();
    void ChangeStateGroup(MorphoStateGroup _Group, const wstring& _State = L"");
    void RepeatState(wstring _State = L"");
    void ProcPatternStep();
    void OnWeaponLTrigger();
    void OnWeaponRTrigger();
    void OffWeaponLTrigger();
    void OffWeaponRTrigger();

private:
    void ChangeStateGroup_Random(MorphoStateGroup _Group);
    void ChangeStateGroup_Set(MorphoStateGroup _Group, const wstring& _State);
    void AddGroupPublicState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State);
    void AddGroupPrivateState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State);

public:
    void SetPattern(MorphoPatternType _Pattern);
    void SetPhase(UINT _Phase) { m_Phase = _Phase; }
    void ClearComboLevel() { m_ComboLevel = 0; }
    void AddComboLevel() { m_ComboLevel++; }
    void SetComboLevel(UINT _Level) { m_ComboLevel = _Level; }
    void EnableRender();
    void DisableRender();
    void SetTeleportTime(bool _Emissive);
    void ClearEmissive();
    void SetEmissive(Vec3 _Color);
    void SetWeaponEmissive(Vec3 _Color);

    MorphoStateGroup GetCurStateGroup() const { return m_CurStateGroup; }
    UINT GetPhase() const { return m_Phase; }
    bool IsPattern() const { return m_Pattern != MorphoPatternType::NONE; }
    bool IsPattern(MorphoPatternType _Pattern, UINT _Step) const { return m_Pattern == _Pattern && m_PatternStep == _Step; }
    UINT GetPatternStep() const { return m_PatternStep; }
    UINT GetComboLevel() const { return m_ComboLevel; }
    float GetNearDist() const { return m_NearDist; }
    bool IsNearPlayer() const { return GetPlayerDist() <= m_NearDist; }
    float GetPlayerDist() const;
    bool IsGround() const;
    CGameObject* GetWeaponL() const { return m_WeaponL; }
    CGameObject* GetWeaponR() const { return m_WeaponR; }
    const vector<CGameObject*>& GetShockWave() const { return m_vecShockWave; }
    float GetEmissiveTime() { return m_EmissiveTime; }
    Vec3 GetMapSize() const { return m_MapSize; }
    Vec3 GetMapFloorOffset() const { return m_MapFloorOffset; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMorphoFSM)
    CMorphoFSM();
    virtual ~CMorphoFSM();
};