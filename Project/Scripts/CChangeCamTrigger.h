#pragma once

#include <Engine/CScript.h>

class CCameraController;

class CChangeCamTrigger : public CScript
{
private:
    CCameraController* m_MainCamController;

    bool m_bIsReturn;

    bool m_bChangedir;
    bool m_bChangeDist;
    bool m_bChangeRotationSpeed;
    bool m_bChangeZoomspeed;

    Vec3 m_ChangeLookDir;
    float m_ChangeLookDist;
    float m_RotationSpeed;
    float m_ZoomSpeed;

    Vec3 m_UndoDir;
    float m_UndoDist;
    float m_UndoRotataionSpeed;
    float m_UndoZoomSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    CLONE(CChangeCamTrigger);

public:
    CChangeCamTrigger();
    CChangeCamTrigger(const CChangeCamTrigger& _Origin);
    virtual ~CChangeCamTrigger();
};
