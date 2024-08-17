#pragma once
#include <Engine\\CScript.h>

class CKirbyBulletSize : public CScript
{
private:
    int m_SizeType;

public:
    int GetSizeType() { return m_SizeType; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyBulletSize);
    CKirbyBulletSize();
    CKirbyBulletSize(const CKirbyBulletSize& _Origin);
    virtual ~CKirbyBulletSize();
};
