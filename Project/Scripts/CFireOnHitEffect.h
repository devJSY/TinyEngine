#pragma once

#include <Engine/CScript.h>

enum class FireEffect_Step
{
    LV0,
    LV1,
    LV2,
    END,
};


class CFireOnHitEffect : public CScript
{
private:
    UINT m_Step; // Fire 단계(소, 중, 대)

    bool m_bDestroy;

    int m_AccFireNumber;      // Fire가 누적된 개수
    int m_RequiredFireNumber; // 다음 단계로 가기 위한 누적 불의 개수

    float m_Acc;             // Fire의 지속시간
    float m_Duration;        // Fire의 최대 지속 시간
    float m_DestroyDuration; // Object 파괴까지의 지속 시간

private:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFireOnHitEffect);
    CFireOnHitEffect();
    CFireOnHitEffect(const CFireOnHitEffect& Origin);
    virtual ~CFireOnHitEffect();

};
