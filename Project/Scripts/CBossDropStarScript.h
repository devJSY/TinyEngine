#pragma once
#include <Engine\\CScript.h>

class CBossDropStarScript : public CScript
{
private:
    CGameObject* m_Star;
    CLight* m_Light;
    Vec3 m_OriginScale;
    float m_LifeTime;
    float m_AccTime;
    float m_LightAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CBossDropStarScript);
    CBossDropStarScript();
    virtual ~CBossDropStarScript();
};