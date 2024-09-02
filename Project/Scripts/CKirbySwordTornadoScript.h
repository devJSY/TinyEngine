#pragma once
#include <Engine\\CScript.h>

class CKirbySwordTornadoScript : public CScript
{
private:
    Vec3 m_OriginPosition;
    Vec3 m_OriginFront;
    Vec3 m_OriginRight;
    Vec3 m_OriginScale;
    float m_AccTime;
    float m_PlayTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetOriginRight(Vec3 _Right) { m_OriginRight = _Right; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbySwordTornadoScript);
    CKirbySwordTornadoScript();
    CKirbySwordTornadoScript(const CKirbySwordTornadoScript& origin);
    virtual ~CKirbySwordTornadoScript();
};
