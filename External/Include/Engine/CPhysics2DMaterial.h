#pragma once
#include "CAsset.h"

class CPhysics2DMaterial : public CAsset
{
private:
    float m_Friction;
    float m_Bounciness;

public:
    float GetFriction() const { return m_Friction; }
    void SetFriction(float _Friction) { m_Friction = _Friction; };

    float GetBounciness() const { return m_Bounciness; }
    void SetBounciness(float _Bounciness) { m_Bounciness = _Bounciness; };

public:
    CLONE(CPhysics2DMaterial);

    virtual int Save(const wstring& _strRelativePath) override;
    virtual int Load(const wstring& _strFilePath) override;

    CPhysics2DMaterial(bool _bEngineAsset = false);
    virtual ~CPhysics2DMaterial();

    friend class CPhysics2DMgr;
};
