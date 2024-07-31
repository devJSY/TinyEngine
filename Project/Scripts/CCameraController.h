#pragma once

#include <Engine/CScript.h>

enum class CameraSetup
{
    NORMAL,
    PROGRESS,
    TWOTARGET,

    ELFILIS_AIR,

};

class CCameraController : public CScript
{
private:
    CameraSetup                 m_Setup;             // 현재 카메라의 Setup

    // Target
    CGameObject*                m_Target;               // 카메라가 바라봐야하는 타겟
    Vec3                        m_TargetPos;            // 현재 프레임에서 타겟의 위치
    Vec3                        m_TargetOffset;     // 타겟으로부터의 오프셋

    CGameObject*                m_SubTarget;            // SubTarget
    Vec3                        m_SubTargetPos;         // SubTarget의 위치
    Vec3                        m_SubTargetOffset;      // SubTarget Offset

    Vec3                        m_Offset;               // LookAtPos에 대한 Offset

    // 설정 값
    Vec3                        m_LookEyePos;           // 카메라가 있어야 하는 위치
    Vec3                        m_LookAtPos;            // 카메라가 바라봐야하는 위치
    Vec3                        m_LookDir;              // 카메라가 바라보는 각도
    float                       m_LookDist;             // 카메라와 타겟 사이의 거리

    float                       m_MinSpeed;             // 카메라의 최소 스피드
    float                       m_MaxSpeed;             // 카메라의 최대 스피드
    float                       m_ThresholdDistance;    // 카메라가 최대 스피드로 이동하기 위한 임계 거리

    float                       m_RotationSpeed;        // 회전 속도


    float                       m_ZoomMinSpeed;         // Zoom 최소 속도
    float                       m_ZoomMaxSpeed;         // Zoom 최대 속도
    float                       m_ZoomThreshold;        // Zoom 임계 거리

    // 현재 프레임
    Vec3                        m_CurLookDir;           // 현재 프레임에서 카메라의 각도
    Vec3                        m_CurLookAtPos;         // 현재 프레임에서 카메라가 바라봐야하는 위치
    float                       m_CurDistance;          // 현재 프레임에서 카메라와 LookAt의 거리

    // 이전 프레임
    Vec3                        m_PrevLookDir;          // 이전 프레임에서 카메라의 각도
    Vec3                        m_PrevLookAtPos;        // 이전 프레임에서 카메라가 바라봐야하는 위치
    float                       m_PrevDistance;         // 이전 프레임에서 카메라와 LookAt의 거리

    // Progress Setup
    Vec3                        m_ProgressStartOffset;
    Vec3                        m_ProgressEndOffset;
    Vec3                        m_ProgressStartPos;
    Vec3                        m_ProgressStartDir;
    float                       m_ProgressStartDist;
    Vec3                        m_ProgressEndPos;
    Vec3                        m_ProgressEndDir;
    float                       m_ProgressEndDist;

    // TwoTarget
    float                       m_DistanceOffset;       // 두 물체의 Pos를 기준으로 잡기 때문에 두 물체의 매쉬가 온전히 보이려면 Offset 만큼 Distance에 더 해줘야 한다.
    
    //Edit
    bool                        m_EditMode;             // EditMode 스위치
    float                       m_EditRotSpeed;         // EditMode에서의 카메라 회전 조절 속도
    float                       m_EditZoomSpeed;        // EditMode에서의 LookDist 수정 속도(실제 Zoom속도가 달라지진 않음)

 public:
    CameraSetup GetCameraSetup() const { return m_Setup; }
    CGameObject* GetMainTarget() const { return m_Target; }
    CGameObject* GetSubTarget() const { return m_SubTarget; }
    Vec3 GetOffset() const { return m_Offset; }
    Vec3 GetMainTargetOffset() const { return m_TargetOffset; }
    Vec3 GetSubTargetOffset() const { return m_SubTargetOffset; }
    Vec3 GetLookDir() const { return m_LookDir; }
    float GetLookDist() const { return m_LookDist; }
    float GetMinSpeed() const { return m_MinSpeed; }
    float GetMaxSpeed() const { return m_MaxSpeed; }
    float GetThresholdDistance() const { return m_ThresholdDistance; }
    float GetRotationSpeed() const { return m_RotationSpeed; }
    float GetZoomMinSpeed() const { return m_ZoomMinSpeed; }
    float GetZoomMaxSpeed() const { return m_ZoomMaxSpeed; }
    float GetZoomThreshold() const { return m_ZoomThreshold; }

    void SetCameraSetup(CameraSetup _Setup) { m_Setup = _Setup; }
    void SetMainTarget(CGameObject* _Target) { m_Target = _Target; } 
    void SetMainTarget(wstring _TargetName);
    void SetSubTarget(CGameObject* _SubTarget) { m_SubTarget = _SubTarget; }
    void SetSubTarget(wstring _TargetName);
    void SetOffset(Vec3 _Offset) { m_Offset = _Offset; }
    void SetTargetOffset(Vec3 _TargetOffset) { m_TargetOffset = _TargetOffset; }
    void SetSubTargetOffset(Vec3 _TargetOffSet) { m_SubTargetOffset = _TargetOffSet; }
    void SetLookDir(Vec3 _LookDir) { m_LookDir = _LookDir; }
    void SetLookDist(float _LookDist) { m_LookDist = _LookDist; }
    void SetMinSpeed(float _MinSpeed) { m_MinSpeed = _MinSpeed; }
    void SetMaxSpeed(float _MaxSpeed) { m_MaxSpeed = _MaxSpeed; }
    void SetThresholdDistance(float _Threshold) { m_ThresholdDistance = _Threshold; }
    void SetRotationSpeed(float _RotationSpeed) { m_RotationSpeed = _RotationSpeed; }
    void SetZoomMinSpeed(float _MinSpeed) { m_ZoomMinSpeed = _MinSpeed; }
    void SetZoomMaxSpeed(float _MaxSpeed) { m_ZoomMaxSpeed = _MaxSpeed; }
    void SetZoomThreshold(float _Threshold) { m_ZoomThreshold = _Threshold; } 
    void SetDistanceOffset(float _DistOffset) { m_DistanceOffset = _DistOffset; } 

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void UpdateTargetPos(); 
    void ApplyTargetOffset();
    void SetUpProc();
    void ApplyOffset();
    void UpdateLookAtPos();
    void UpdateLookDir();
    void UpdateLookDistance();
    
    Vec3 CalCamPos(Vec3 _TargetWorldPos, Vec3 _LookDir, float _CamDist);
    
    void Normal();
    void Progress();
    void TwoTarget();
    void Elfilis_Air();
    // EditMode
    void EditMode();

public:
    void ResetCamera(); // 현재의 설정값들로 카메라를 바로 이동한다.
    void ChangeMainTarget(CGameObject* _Target); // Target 변경 후 카메라 강제 이동
    void ChangeMainTarget(wstring _TargetName); // Target 변경 후 카메라 강제 이동 
    void ChangeLookSetting(Vec3 _LookDir, float _LookDist); // Look Setting 변경
    void ChangeFollwSpeedSetting(float _MinSpeed, float _MaxSpeed, float _Threshold); // Follow Setting 변경
    void ProgressSetup(Vec3 _StartPos, Vec3 _EndPos,Vec3 _StartOffset, Vec3 _EndOffset, Vec3 _StartDir, Vec3 _EndDir, float _StartDist, float _EndDist); // Progress로 Camera Setup 상태 변경
    void TwoTarget(CGameObject* _SubTarget, bool _bChangeLookDir, Vec3 _LookDir, float _DistanceOffset);
    void TwoTarget(wstring _SubTargetName, Vec3 _LookDir, float _DistanceOffset);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CCameraController);
    CCameraController();
    virtual ~CCameraController();
};
