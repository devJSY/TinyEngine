#pragma once
#include <Engine\\CScript.h>

class CPlayerDamageLifeBarScript : public CScript
{
private:
    float m_RenderLife;
    float m_DelayTime;

public:
    void TakeHit(float _DelayTime) { m_DelayTime = _DelayTime; }

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPlayerDamageLifeBarScript);

public:
    CPlayerDamageLifeBarScript();
    virtual ~CPlayerDamageLifeBarScript();
};
