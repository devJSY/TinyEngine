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
    CGameObject* m_Player;
    float m_CamSpeed;
    float m_CamMoveRangeX;
    float m_CamMoveRangeY;
    Vec3 m_OffsetPos;

    std::list<tCamEffect> m_listCamEffect;

    bool m_bLocked;
    Vec3 m_LockedPos;

public:
    virtual void begin() override;
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

    void Lock(bool _bLocked, Vec3 _LockedPos = Vec3());

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerCameraScript);

public:
    CPlayerCameraScript();
    CPlayerCameraScript(const CPlayerCameraScript& origin);
    virtual ~CPlayerCameraScript();
};
