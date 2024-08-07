#pragma once
#include <Engine/CScript.h>
class CCameraController;

class CFixedViewCamTrigger : public CScript
{
private:
    CCameraController* m_MainCamController;

    bool m_bIsImmediate;
    Vec3 m_FixedViewPos;

    bool m_Undo;
    Vec3 m_ExitLookDir;
    float m_ExitLookDist;
    bool m_bExitImmediate;


public:
    virtual void begin() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    CLONE(CFixedViewCamTrigger);

public:
    CFixedViewCamTrigger();
    CFixedViewCamTrigger(const CFixedViewCamTrigger& _Origin);
    virtual ~CFixedViewCamTrigger();
};

