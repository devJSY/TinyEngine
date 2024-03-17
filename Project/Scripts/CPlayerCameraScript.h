#pragma once
#include <Engine\\CScript.h>

class CPlayerCameraScript : public CScript
{
private:
    Vec3 m_OffsetPos;

public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};
    
    CLONE(CPlayerCameraScript);

public:
    CPlayerCameraScript();
    virtual ~CPlayerCameraScript();
};
