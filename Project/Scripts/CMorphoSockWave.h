#pragma once
#include <Engine\\CScript.h>

class CMorphoSockWave : public CScript
{
public:
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoSockWave);
    CMorphoSockWave();
    virtual ~CMorphoSockWave();
};