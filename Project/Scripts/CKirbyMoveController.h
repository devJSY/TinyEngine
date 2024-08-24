#pragma once

#include <Engine/CScript.h>



// 방향 변경 타입 (enum 순서가 우선순위)
enum class ForceDirType
{
    STAGEEVENT,
    STATE,
    DEFORM,
    HIT,
    END,
};

// 물체의 방향을 강제로 바꿔야하는 경우
struct ForceDirInfo
{
    ForceDirType Type;
    Vec3 Dir; // 바라봐야할 방향(World 좌표계)
    bool Immediate;
};

class CKirbyMoveController : public CScript
{
private:
    // 입력
    Vec3                        m_Input;
    Vec3                        m_InputWorld;
    bool                        m_bGround;
    bool                        m_bJump;
    bool                        m_bActiveFriction;
    bool                        m_bForwardMode;
    bool                        m_bTeleportGround;

    // Lock
    bool                        m_bInputLock;
    bool                        m_bMoveLock;
    bool                        m_bJumpLock;
    bool                        m_bDirLock;
    bool                        m_bLimitFallSpeed;

    // 방향
    Vec3                        m_CurDir;
    Vec3                        m_MoveDir;
    Vec3                        m_TowardDir;
    vector<ForceDirInfo>        m_ForceDirInfos;

    // 물리
    Vec3                        m_MoveVelocity;
    Vec3                        m_AddVelocity;
    Vec3                        m_Accel;
    float                       m_Speed;
    float                       m_MaxSpeed;
    float                       m_MaxFallSpeed;
    float                       m_RotSpeed;
    float                       m_Friction; // 마찰력 계수
    float                       m_JumpPower;
    float                       m_Gravity;

    // Ray 정보
    RaycastHit                  m_RayHit;

    float                       m_HoveringLimitHeight;
    float                       m_HoveringMinSpeed;
    Vec3                        m_JumpPos;

    // Respawn 정보
    Vec3                        m_CheckPointPos;
    Vec3                        m_ForcePos;

    float                       m_InitGravity;


public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Input();
    void SetDir();
    void RayGround();
    void Move();

public:
    void LockInput() { m_bInputLock = true; }
    void UnlockInput() { m_bInputLock = false; }
    void LockMove() { m_bMoveLock = true; }
    void UnlockMove() { m_bMoveLock = false; }
    void LockJump() { m_bJumpLock = true; }
    void UnlockJump() { m_bJumpLock = false; }
    void LockDirection() { m_bDirLock = true; }
    void UnlockDirection() { m_bDirLock = false; }
    void SetForwardMode(bool _Mode) { m_bForwardMode = _Mode; }
    void SetLimitFallSpeed(bool _b) { m_bLimitFallSpeed = _b; }

    void Jump() { m_bJump = true; }
    void VelocityCut(float _f) { _f == 0.f ? m_MoveVelocity.y = 0.f : m_MoveVelocity.y /= _f; }
    void SetFrictionMode(bool _Friction) { m_bActiveFriction = _Friction; }
    void SetSpeed(float _Speed) { m_Speed = _Speed; }
    void SetRotSpeed(float _Speed) { m_RotSpeed = _Speed; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetGravity(float _Gravity) { m_Gravity = _Gravity; }
    void ClearVelocityY() { m_MoveVelocity.y = 0.f; }
    void ClearVelocityHorizontal()
    {
        m_MoveVelocity.x = 0.f;
        m_MoveVelocity.z = 0.f;
    }
    void SetVelocity(Vec3 _VeloCity) { m_MoveVelocity = _VeloCity; }
    void AddVelocity(Vec3 _AddVel) { m_AddVelocity += _AddVel; }
    void ForceDir(ForceDirInfo _Info) { m_ForceDirInfos.push_back(_Info); }
    void ForcePos(Vec3 _ForcePos) { m_ForcePos = _ForcePos; }
    void SetJumpPower(float _Power) { m_JumpPower = _Power; }
    void SetCheckPoint(Vec3 _CheckPoint) { m_CheckPointPos = _CheckPoint; }
    void TeleportGround() { m_bTeleportGround = true; }

    Vec3 GetInput() const { return m_Input; }
    Vec3 GetInputWorld() const { return m_InputWorld; }
    Vec3 GetMoveDir() const { return m_MoveDir; }
    Vec3 GetVelocity() const { return m_MoveVelocity; }
    Vec3 GetCheckPoint() const { return m_CheckPointPos; }
    float GetSpeed() const { return m_Speed; }
    float GetRotSpeed() const { return m_RotSpeed; }
    float GetGravity() const { return m_Gravity; }
    float GetGuard() const { return m_bActiveFriction; }
    float GetJumpPower() const { return m_JumpPower; }
    bool IsGround() const { return m_bGround; }
    float GetInitGravity() const { return m_InitGravity; }
    RaycastHit GetRay() const { return m_RayHit; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyMoveController);
    CKirbyMoveController();
    virtual ~CKirbyMoveController();
};
