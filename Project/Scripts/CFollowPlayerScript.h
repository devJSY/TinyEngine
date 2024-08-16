#pragma once
#include <Engine\\CScript.h>

class CFollowPlayerScript : public CScript
{
private:
    Vec3 m_PosOffset;
    Vec3 m_RotOffset;
    bool m_bPosOffset;
    bool m_bRotOffset;
    bool m_bFollowPos[3];
    bool m_bFollowRot[3];

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFollowPlayerScript);
    CFollowPlayerScript();
    CFollowPlayerScript(const CFollowPlayerScript& _Origin);
    virtual ~CFollowPlayerScript();
};