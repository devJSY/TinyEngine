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
    AtkGroundTeleport,
    AtkAir,

    END,
};

class CMorphoFSM : public CFSMScript
{
private:
    // FSM
    map<MorphoStateGroup, vector<wstring>[2]> m_StateGroup;  // ( StateGroup, States )
    MorphoStateGroup m_CurStateGroup;
    UINT m_Phase;
    UINT m_ComboLevel;
    float m_NearDist;
    bool m_bAttackRepeat;

    // map
    Vec3 m_MapFloorOffset;
    Vec3 m_MapSize;

    // shockwave
    vector<CGameObject*> m_vecShockWave;

public:
    virtual void begin() override;
    virtual void tick() override;

    void Move();
    void Attack();
    void ChangeStateGroup(MorphoStateGroup _Group, const wstring& _State = L"");
    void RepeatState(wstring _State = L"");

private:
    void ChangeStateGroup_Random(MorphoStateGroup _Group);
    void ChangeStateGroup_Set(MorphoStateGroup _Group, const wstring& _State);
    void AddGroupPublicState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State);
    void AddGroupPrivateState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State);

public:
    void ClearComboLevel() { m_ComboLevel = 0; }
    void AddComboLevel() { m_ComboLevel++; }
    void SetComboLevel(UINT _Level) { m_ComboLevel = _Level; }

    MorphoStateGroup GetCurStateGroup() const { return m_CurStateGroup; }
    UINT GetPhase() const { return m_Phase; }
    UINT GetComboLevel() const { return m_ComboLevel; }
    float GetNearDist() const { return m_NearDist; }
    bool IsNearPlayer() const { return GetPlayerDist() <= m_NearDist; }
    Vec3 GetMapSize() const { return m_MapSize; }
    Vec3 GetMapFloorOffset() const { return m_MapFloorOffset; }
    float GetPlayerDist() const;
    const vector<CGameObject*>& GetShockWave() const { return m_vecShockWave; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMorphoFSM)
    CMorphoFSM();
    virtual ~CMorphoFSM();
};