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

    bool m_bLerpTrack;

    /*****************
    | Tracking Value
    ******************/
    float m_fLerpValue;
    float m_fTrackDistance;
    bool m_bIsTrack;
    bool m_bIsTrackObject;

    /*****************
    | Rush Value
    ******************/
    float m_fMinSpeed;
    float m_fMaxSpeed;
    float m_fCurSpeed;

    float m_fRushLerp;

    bool m_bIsRush;
    bool m_bIsRushObject;
    bool m_bIsSlip;

    /****************
    | Patrol Value
    *****************/
    Vec3 m_vPatrolDir;

    Vec3 m_pOriginPos;
    Vec3 m_pDestPos;

    bool m_bIsPatrolObject;
    bool m_bIsPatrol;

    bool m_bLoopPatrol;

    /****************
    | Rotating Value
    *****************/
    bool m_bIsRotateObject;
    bool m_bIsRotating;

public:
    float GetTrackDist() const { return m_fTrackDistance; }
    void SetTrack(const bool _flag)  { m_bIsTrack = _flag; }

    void SetRush(const bool _flag) { m_bIsRush = _flag; }

public:	
    virtual void begin() override;
    virtual void tick() override;

private:
    void Tracking(bool _flag);
    void Rush(bool _flag);
    void Slip(bool _flag);
    void Patrol(bool _flag);

    void Rotating();

private:
    void ApplyDir(Vec3 _vFront, bool _flag);
    Vec3 CaculateDir(Vec3 _vRadian);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    CLONE(CMonsterMoveController)
    CMonsterMoveController();
    virtual ~CMonsterMoveController();
};
