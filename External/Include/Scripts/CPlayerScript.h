#pragma once
#include <Engine\\CScript.h>

class CPlayerScript : public CScript
{
private:
    Vec3 m_MoveVelocity;
    float m_Speed;
    float m_ForcePower;
    float m_JumpPower;
    float m_RayCastDist;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnControllerColliderHit(struct ControllerColliderHit Hit);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerScript);

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& origin); // 멤버 변수 AddScriptParam() 등록시 주소를 전달하기 때문에 복사생성자를 구현해줘야함
    virtual ~CPlayerScript();
};
