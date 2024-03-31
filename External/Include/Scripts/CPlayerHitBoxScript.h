#pragma once
#include <Engine\\CScript.h>

class CPlayerHitBoxScript : public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetEnabled(bool _bEnable);

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
