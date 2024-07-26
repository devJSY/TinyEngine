#pragma once

#include <Engine/CScript.h>

enum class CameraMode
{
    NORMAL,

};

class CCameraController : public CScript
{
private:
    CGameObject*                m_Target;               // 카메라가 바라봐야하는 타겟

    // 설정 값
    Vec3                        m_LookEyePos;           // 카메라가 있어야 하는 위치
    Vec3                        m_LookDir;              // 카메라가 바라보는 각도
    float                       m_LookDist;             // 카메라와 타겟 사이의 거리

    float                       m_MinSpeed;             // 카메라의 최소 스피드
    float                       m_MaxSpeed;             // 카메라의 최대 스피드
    float                       m_ThresholdDistance;    // 카메라가 최대 스피드로 이동하기 위한 임계 거리

    float                       m_RotationSpeed;        // 회전 속도

    // 현재 프레임
    Vec3                        m_CurLookDir;           // 현재 프레임에서 카메라의 각도
    Vec3                        m_CurLookAtPos;         // 현재 프레임에서 카메라가 바라봐야하는 위치
    Vec3                        m_TargetPos;            // 현재 프레임에서 타겟의 위치

    // 이전 프레임
    Vec3                        m_PrevLookDir;          // 이전 프레임에서 카메라의 각도
    Vec3                        m_PrevLookAtPos;        // 이전 프레임에서 카메라가 바라봐야하는 위치

    //Edit
    bool                        m_EditMode;             // EditMode 스위치
    float                       m_EditRotSpeed;         // EditMode에서의 카메라 회전 조절 속도
    float                       m_EditZoomSpeed;        // EditMode에서의 카메라 거리 조절 속도

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void UpdateTargetPos(); 
    void UpdateLookAtPos();
    void UpdateLookDir();
    
    Vec3 CalCamPos(Vec3 _TargetWorldPos, Vec3 _LookDir, float _CamDist);
    
    // EditMode
    void EditMode();


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CCameraController);
    CCameraController();
    virtual ~CCameraController();
};
