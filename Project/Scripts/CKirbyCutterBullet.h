#pragma once
#include <Engine/CScript.h>

enum class BuningStep
{
    NONE,
    STEP_1,
    STEP_2,
    STEP_3,
    END,
};

enum class BulletState
{
    HOLD_AIR, // 가만히 도는 상태
    HOLD_WALL, // 벽에 부딪힌 상태
    DRAG, // 감속을 받는 상태(Kirby한테 돌아가지 x)
    BACK, // 커비를 향해 날아가는 상태
    RAGE, // 빠르게 움직이는 상태(Kirby를 만나면 회수되지만, Kirby를 쫓아가진 않는다)
    BURNING_WAIT,
    BURNING,
};

class CKirbyCutterBullet : public CScript
{
private:
    BulletState         m_CurState;
    BuningStep          m_CurStep;

    bool                m_IsBack;           // 커비랑 충돌시 회수

    Vec3                m_MoveDir;          //  움직이는 방향
    Vec3                m_PrevDir;
    float               m_Speed;            //  속도
    float               m_BurningSpeed;     //  Burning상태 스피드

    Vec3                m_Velocity;         //  현재 속도

    int                 m_BounceCount;      //  남은 튕기는 횟수

    float               m_Duration;         //  상태 최대시간
    float               m_Acc;              //  상태 지속시간

    float               m_Frequency;        //  좌우로 흔들리는 주기
    float               m_RotAcc;           
    float               m_Angle;            //  좌우로 흔들리는 크기

    bool                m_bPressedATK;      
    bool                m_bBuning;
    float               m_BuningStepUpDuration;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetState(BulletState _State);
    void SetDir(Vec3 _Dir) { m_MoveDir = _Dir; }

private:
    bool StepUp();


private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider) override;
    virtual void OnCollisionStay(CCollider* _OtherCollider){}
    virtual void OnCollisionExit(CCollider* _OtherCollider){}

    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider);


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyCutterBullet);
    CKirbyCutterBullet();
    virtual ~CKirbyCutterBullet();

};
