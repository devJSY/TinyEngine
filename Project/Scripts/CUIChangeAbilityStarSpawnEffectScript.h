#pragma once
#include <Engine\CScript.h>

enum class UIChangeAbilityStarSpawnEffectState
{
    Enter,
    Progress,
    End,
};

class CUIChangeAbilityStarSpawnEffectScript : public CScript
{
private:
    Vec3 m_vCenterPosOffset;
    Vec3 m_vCenterPos;

    Vec3 m_vBigStarScale;
    Vec3 m_vLittleStarScale;

    UIChangeAbilityStarSpawnEffectState m_eState;
    
    float m_fOriginRadius;
    float m_fBigStarRadiusSpeed;
    float m_fLitttleStarRadiusSpeed;
    float m_fThetaSpeed;
    float m_fSpawnStarDeleteTime;
    float m_fThreshHoldStar;
    float m_fStarSpawnTime;
    float m_fBeginDegree;

public:
    virtual void tick() override;

private:
    void ChangeState(UIChangeAbilityStarSpawnEffectState _eState) { m_eState = _eState; }
    void Enter();
    void Progress();
    void End();

    void CaculateTargetPos();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIChangeAbilityStarSpawnEffectScript)

public:
    CUIChangeAbilityStarSpawnEffectScript();
    CUIChangeAbilityStarSpawnEffectScript(const CUIChangeAbilityStarSpawnEffectScript& Origin);
    virtual ~CUIChangeAbilityStarSpawnEffectScript();
};
