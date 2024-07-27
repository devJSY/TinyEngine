#pragma once

#include <Engine/CScript.h>

enum class CameraSetup
{
    NORMAL,
    PROGRESS,
};

class CCameraController : public CScript
{
private:
    CameraSetup                 m_CurSetup;             // 현재 카메라의 Setup

    // Target
    CGameObject*                m_Target;               // 카메라가 바라봐야하는 타겟
    Vec3                        m_TargetPos;            // 현재 프레임에서 타겟의 위치
    Vec3                        m_Offset;               // 타겟으로부터의 오프셋

    // 설정 값
    Vec3                        m_LookEyePos;           // 카메라가 있어야 하는 위치
    Vec3                        m_LookDir;              // 카메라가 바라보는 각도
    float                       m_LookDist;             // 카메라와 타겟 사이의 거리

    float                       m_MinSpeed;             // 카메라의 최소 스피드
    float                       m_MaxSpeed;             // 카메라의 최대 스피드
    float                       m_ThresholdDistance;    // 카메라가 최대 스피드로 이동하기 위한 임계 거리

    float                       m_RotationSpeed;        // 회전 속도
    float                       m_ZoomSpeed;            // Zoom 속도

    // 현재 프레임
    Vec3                        m_CurLookDir;           // 현재 프레임에서 카메라의 각도
    Vec3                        m_CurLookAtPos;         // 현재 프레임에서 카메라가 바라봐야하는 위치
    float                       m_CurDistance;          // 현재 프레임에서 카메라와 LookAt의 거리

    // 이전 프레임
    Vec3                        m_PrevLookDir;          // 이전 프레임에서 카메라의 각도
    Vec3                        m_PrevLookAtPos;        // 이전 프레임에서 카메라가 바라봐야하는 위치
    float                       m_PrevDistance;         // 이전 프레임에서 카메라와 LookAt의 거리

    // Progress Setup
    Vec3                        m_ProgressStartPos;
    Vec3                        m_ProgressStartDir;
    float                       m_ProgressStartDist;
    Vec3                        m_ProgressEndPos;
    Vec3                        m_ProgressEndDir;
    float                       m_ProgressEndDist;
    
    //Edit
    bool                        m_EditMode;             // EditMode 스위치
    float                       m_EditRotSpeed;         // EditMode에서의 카메라 회전 조절 속도
    float                       m_EditZoomSpeed;        // EditMode에서의 카메라 거리 조절 속도

public:
    virtual void begin() override;
    virtual void tick() override;


private:
    void SetUpProc();
    void UpdateTargetPos(); 
    void UpdateLookAtPos();
    void UpdateLookDir();
    void UpdateLookDistance();
    
    Vec3 CalCamPos(Vec3 _TargetWorldPos, Vec3 _LookDir, float _CamDist);
    
    // EditMode
    void EditMode();
    void Progress();

public:
    //void ChangeTarget();
    //void ChangeSetting(Vec3 _LookDir, float CamDist, );
    //void ProgressSetup(Vec3 _StartPos, Vec3 _EndPos, Vec3 _StartDir, Vec3 _EndDir, float _StartDist, float _EndDist);


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CCameraController);
    CCameraController();
    virtual ~CCameraController();
};
