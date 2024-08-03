#pragma once
#include <Engine\\CScript.h>

class CElfilisLaser : public CScript
{
private:
    int m_Step;

public:
    virtual void tick() override;

    void SetStart();
    void SetWait();
    void SetEnd();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisLaser);
    CElfilisLaser();
    virtual ~CElfilisLaser();
};
