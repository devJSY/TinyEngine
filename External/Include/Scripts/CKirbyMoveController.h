#pragma once

#include <Engine/CScript.h>

// 방향 변경 타입 (enum 순서가 우선순위)
enum class ForceDirType
{
    DEFORM,
    HIT,
    END,
};

enum class AddForceType
{
    Acceleration,   // 질량을 무시하고 강체에 즉각적인 가속도 추가
    VelocityChange, // 질량을 무시하고 강체에 즉각적인 속도 변화 추가
};

// 물체의 방향을 강제로 바꿔야하는 경우
struct ForceDirInfo
{
    ForceDirType Type;
    Vec3 Dir; // 바라봐야할 방향(World 좌표계)
};


class CKirbyMoveController : public CScript
{
private:
    // 입력
    Vec3                        m_Input;
    bool                        m_bMoveLock;
    bool                        m_bJumpLock;
    bool                        m_bDirLock;

    // 방향
    Vec3                        m_CurDir;
    Vec3                        m_MoveDir;
    Vec3                        m_TowardDir;
    Vec3                        m_GroundNormal;
    vector<ForceDirInfo>        m_ForceDirInfos;

    // 물리
    Vec3                        m_MoveVelocity;
    Vec3                        m_AdditionalVelocity;
    float                       m_Speed;
    float                       m_RotSpeed;
    float                       m_JumpPower;
    float                       m_RayCastDist;
    float                       m_Gravity;
    float                       m_HoveringLimitHeight;
    float                       m_HoveringHeight;



private:
    virtual void OnControllerColliderHit(struct ControllerColliderHit Hit);

public:
    virtual void begin() override;
    virtual void tick() override;
    void AddForce(Vec3 _Force, AddForceType _Type);

public:
    Vec3 GetInput() const { return m_Input; }
    Vec3 GetMoveDir() const { return m_MoveDir; }
    float GetGravity() const { return m_Gravity; }

    void LockMove() { m_bMoveLock = true; }
    void UnlockMove() { m_bMoveLock = false; }
    void LockJump() { m_bJumpLock = true; }
    void UnlockJump() { m_bJumpLock = false; }
    void LockDirection() { m_bDirLock = true; }
    void UnlockDirection() { m_bDirLock = false; }
    void ClearHoveringHeight() { m_HoveringHeight = 0.f; }
    void ClearVelocityY() { m_MoveVelocity.y = 0.f; }
    void SetGravity(float _Gravity) { m_Gravity = _Gravity; }

private:
    void Input();
    void SetDir();
    void Move();
    void SurfaceAlignment();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyMoveController);
    CKirbyMoveController();
    CKirbyMoveController(const CKirbyMoveController& _Origin);
    virtual ~CKirbyMoveController();
};
