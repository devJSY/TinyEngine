#pragma once

#include <Engine/CScript.h>

class CCameraController;

class CTwoTargetCamTrigger : public CScript
{
private:
    CCameraController* m_MainCamController;

    string m_SubTargetName;

    bool m_bChangeDir;
    Vec3 m_LookDir;

    bool m_bChangeRotationSpeed;
    float m_RotationSpeed;
    float m_UndoRotataionSpeed;

    bool m_bChangeZoomspeed;
    float m_ZoomSpeed;
    float m_UndoZoomSpeed;

    float m_DistanceOffset;

    Vec3 m_ExitDir;
    float m_ExitDistance;

public:
    virtual void begin() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    CLONE(CTwoTargetCamTrigger);

public:
    CTwoTargetCamTrigger();
    CTwoTargetCamTrigger(const CTwoTargetCamTrigger& _Origin);
    virtual ~CTwoTargetCamTrigger();

};
