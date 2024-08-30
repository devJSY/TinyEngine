#pragma once

#include <Engine/CScript.h>

class CKirbySoundListener : public CScript
{
private:
    CGameObject* m_MainCamera;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbySoundListener);
    CKirbySoundListener();
    CKirbySoundListener(const CKirbySoundListener& _Origin);
    virtual ~CKirbySoundListener();

};
