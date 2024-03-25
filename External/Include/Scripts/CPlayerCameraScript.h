#pragma once
#include <Engine\\CScript.h>

enum class CAM_EFFECT
{
    Shake,
    None
};

enum class ShakeDir
{
    Vertical,
    Horizontal,
    Comprehensive,
    None
};

struct tCamEffect
{
    CAM_EFFECT eEffect;
    ShakeDir eShakeDir;
    float fDuration;
    float fCurTime;
    float fDistance;
    float fSpeed;
};

class CPlayerCameraScript : public CScript
{
private:
    float m_CamSpeed;
    float m_CamMoveRangeX;
    float m_CamMoveRangeY;
    Vec3 m_OffsetPos;

    std::list<tCamEffect> m_listCamEffect;

public:
    virtual void tick() override;

public:
    void ShakeCam(ShakeDir _eShakeDir, float _fDuration, float _fDistance, float _fSpeed)
    {
        tCamEffect ef = {};
        ef.eEffect = CAM_EFFECT::Shake;
        ef.eShakeDir = _eShakeDir;
        ef.fDuration = _fDuration;
        ef.fCurTime = 0.f;
        ef.fDistance = _fDistance;
        ef.fSpeed = _fSpeed;

        m_listCamEffect.push_back(ef);
    }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerCameraScript);

public:
    CPlayerCameraScript();
    virtual ~CPlayerCameraScript();
};
