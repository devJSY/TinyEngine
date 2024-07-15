#pragma once
#include <Engine/CScript.h>

class CMonsterMgr;
class CUnitScript;
class CMonsterMoveController : public CScript
{
private:
    CTransform* m_pTransform;
    CMonsterMgr* m_pMgr;
    CUnitScript* m_pUnitScript;

    /*****************
    | Tracking Value
    ******************/
    float m_fLerpValue;
    float m_fTrackDistance;
    bool m_bIsTrack;
    bool m_bIsTrackObject;

public:
    float GetTrackDist() const { return m_fTrackDistance; }
    void SetTrack(const bool _flag)  { m_bIsTrack = _flag; }

public:	
    virtual void begin() override;
    virtual void tick() override;

private:
    void Tracking();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    CLONE(CMonsterMoveController)
    CMonsterMoveController();
    virtual ~CMonsterMoveController();
};
