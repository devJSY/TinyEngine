#pragma once
#include "CFSMScript.h"

enum class MorphoStateGroup
{
    DEMO,

    Move,
    Attack,

    END,
};

class CMorphoFSM : public CFSMScript
{
private:
    // FSM
    UINT m_Phase;
    UINT m_ComboLevel;
    float m_NearDist;
    bool m_bAttackRepeat;

    // map
    Vec3 m_MapFloorOffset;
    Vec3 m_MapSize;

public:
    virtual void begin() override;
    virtual void tick() override;

    void Move();
    void Attack();
    void RepeatState(wstring _State = L"");

public:
    void ClearComboLevel() { m_ComboLevel = 0; }
    void AddComboLevel() { m_ComboLevel++; }

    UINT GetPhase() const { return m_Phase; }
    UINT GetComboLevel() const { return m_ComboLevel; }
    float GetNearDist() const { return m_NearDist; }
    bool IsNearPlayer() const { return GetPlayerDist() <= m_NearDist; }
    Vec3 GetMapSize() const { return m_MapSize; }
    Vec3 GetMapFloorOffset() const { return m_MapFloorOffset; }
    float GetPlayerDist() const;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMorphoFSM)
    CMorphoFSM();
    virtual ~CMorphoFSM();
};