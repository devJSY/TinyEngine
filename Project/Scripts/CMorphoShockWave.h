#pragma once
#include <Engine\\CScript.h>

class CMorphoShockWave : public CScript
{
public:
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoShockWave);
    CMorphoShockWave();
    virtual ~CMorphoShockWave();
};