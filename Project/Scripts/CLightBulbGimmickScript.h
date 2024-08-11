#pragma once
#include <Engine\CScript.h>
class CLightBulbGimmickScript : public CScript
{
public:
    virtual void begin() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CLightBulbGimmickScript)
public:
    CLightBulbGimmickScript();
    virtual ~CLightBulbGimmickScript();
};
