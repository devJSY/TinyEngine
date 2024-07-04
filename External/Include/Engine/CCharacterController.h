#pragma once
#include "CCollider.h"
#include "CPhysicsMgr.h"

class CCharacterController : public CCollider
{
private:
    float m_SlopeLimit;
    float m_StepOffset;
    float m_SkinWidth;
    float m_MinMoveDistance;
    float m_Radius;
    float m_Height;

private:
    float m_MoveElapsedTime;
    bool m_bGrounded; 

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    Vec3 Move(Vec3 _Motion);
    bool IsGrounded() const { return m_bGrounded; }

public:
    float GetSlopeLimit() const { return m_SlopeLimit; }
    void SetSlopeLimit(float _Limit);

    float GetStepOffset() const { return m_StepOffset; }
    void SetStepOffset(float _Offset);

    float GetSkinWidth() const { return m_SkinWidth; }
    void SetSkinWidth(float _Width);

    float GetMinMoveDistance() const { return m_MinMoveDistance; }
    void SetMinMoveDistance(float _Dist);

    float GetRadius() const { return m_Radius; }
    void SetRadius(float _Radius);

    float GetHeight() const { return m_Height; }
    void SetHeight(float _Height);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCharacterController);

public:
    CCharacterController();
    virtual ~CCharacterController();

    friend class CPhysicsMgr;
};
