#pragma once

#include <Engine/CScript.h>

// rigidbody (trigger x) -> 13번 레이어(Fire Bullet)
// Monster_trigger, WorldStatic, FireProjectile Layer와 충돌

enum class Fire_Step
{
    LV0,
    LV1,
    LV2,
    END,
};

class CKirbyFireBullet : public CScript
{
private:
    UINT m_Step;                        // Fire 단계(소, 중, 대)
    
    bool m_bIsFreeze;
    bool m_bDestroy;

    int m_AccFireNumber;                // Fire가 누적된 개수
    int m_RequiredFireNumber;           // 다음 단계로 가기 위한 누적 불의 개수

    float m_Acc;                        // Fire의 지속시간
    float m_Duration;                   // Fire의 최대 지속 시간
    float m_DestroyDuration;            // Object 파괴까지의 지속 시간


    Vec3 m_Dir;

public:
    float GetAcc() { return m_Acc; }
    UINT GetSetp() { return m_Step; }

private:
    void StepUp();
    void StepDown();

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyFireBullet);
    CKirbyFireBullet();
    CKirbyFireBullet(const CKirbyFireBullet& _Origin);
    virtual ~CKirbyFireBullet();

};
