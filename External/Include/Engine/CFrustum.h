#pragma once
#include "CEntity.h"

class CCamera;

enum FACE_TYPE
{
    FT_NEAR,
    FT_FAR,
    FT_LEFT,
    FT_RIGHT,
    FT_TOP,
    FT_BOT,
    FT_END,
};

class CFrustum : public CEntity
{
private:
    CCamera* m_pOwnerCam;
    Vec4 m_arrFace[FT_END];
    Vec3 m_arrProj[8];

public:
    void finaltick();

public:
    bool FrustumCheckByPoint(Vec3 _vWorldPos);
    bool FrustumCheckBySphere(Vec3 _vWorldPos, float _fRadius);

public:
    CLONE(CFrustum);

public:
    CFrustum(CCamera* _pOwnerCam);
    virtual ~CFrustum();
};
