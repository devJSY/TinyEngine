#pragma once
#include <Engine\\CScript.h>

class CPlayerHitBoxScript : public CScript
{
private:
    bool m_bEnable;
    Vec3 m_HitBoxScale;

public:
    virtual void begin() override;

public:
    void SetEnable(bool _bEnable);

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPlayerHitBoxScript);

public:
    CPlayerHitBoxScript();
    virtual ~CPlayerHitBoxScript();
};
