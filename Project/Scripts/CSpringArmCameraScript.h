#pragma once
#include <Engine\\CScript.h>

// ===================================================
// Spring Arm Camera Script
// 카메라가 타겟을 추적하고, 시야가 막히는 상황을 처리
// ===================================================
// Usage: Camera를 Target의 자식으로 두고, Camera에 해당 스크립트를 직접 붙여 사용
// Result: 카메라 오브젝트의 부모가 spring arm object인 것처럼 적용, 카메라의 local transform 변경

class CSpringArmCameraScript : public CScript
{
private:
    // Camera
    Vec3            m_CameraOffset;
    Vec3            m_TargetOffset;
    Vec3            m_ArmRotation;
    float           m_ArmLength;

    // Lag
    float           m_LagSpeed;
    float           m_LagMaxDistance;
    bool            m_bUnableLag;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CSpringArmCameraScript);

public:
    CSpringArmCameraScript();
    CSpringArmCameraScript(const CSpringArmCameraScript& _Origin);
    virtual ~CSpringArmCameraScript();
};