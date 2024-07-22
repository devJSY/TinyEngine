#pragma once
#include <Engine/CScript.h>

enum class JuiceType
{
    Green,
    Yellow,
    Red,
    Purple,
    Orange,
};

enum class JuiceState
{
    Fly,
    Wait,
};

class CCanJuice : public CScript
{
private:
    float m_Acc;
    float m_Duration;
    float m_InitSpeed;
    float m_Damage;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider){};
    virtual void OnCollisionExit(CCollider* _OtherCollider){};

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

public:
    CLONE(CCanJuice);
    CCanJuice();
    virtual ~CCanJuice();
};
