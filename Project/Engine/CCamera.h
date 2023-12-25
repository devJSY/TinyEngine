#pragma once
#include "CComponent.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHIC, // 직교 투영
    PERSPECTIVE,  // 원근 투영
};

class CCamera : public CComponent
{
private:
    PROJ_TYPE m_ProjType; // 투영 방식

    // 원근투영(Perspective)
    float m_FOV; // 시야 각(Filed Of View)

    // 직교투영(Orthographic)
    float m_Width; // 직교투영 가로 길이
    float m_Scale; // 직교투영 배율

    // Both
    float m_AspectRatio; // 종횡비, 투영 가로세로 비율
    float m_Near;        // 투영 최소 거리
    float m_Far;         // 투영 최대 거리

    // 변환 행렬
    Matrix m_matView;
    Matrix m_matProj;

private:
    float m_CamSpeed;

public:
    PROJ_TYPE GetProjType() const { return m_ProjType; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }

    float GetScale() const { return m_Scale; }
    void SetScale(float _Scale) { m_Scale = _Scale; }

    float GetFOV() const { return m_FOV; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

    float GetNear() const { return m_Near; }
    void SetNear(float _near) { m_Near = _near; }

    float GetFar() const { return m_Far; }
    void SetFar(float _far) { m_Far = _far; }

public:
    float GetCameraSpeed() const { return m_CamSpeed; }
    void SetCameraSpeed(float speed) { m_CamSpeed = speed; }

    Matrix GetViewMatrix() const { return m_matView; }
    Matrix GetProjectionMatrix() const { return m_matProj; }

public:
    virtual void finaltick() override;

public:
    CCamera();
    virtual ~CCamera();
};
