#pragma once
#include <Engine\\CScript.h>

class CPlayerScript : public CScript
{
private:
    Ptr<CPrefab> m_ShockWave;
    float m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnCollisionStay(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerScript);

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};
