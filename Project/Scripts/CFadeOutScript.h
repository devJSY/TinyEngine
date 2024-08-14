#pragma once
#include <Engine\\CScript.h>

class CGameObject;

class CFadeOutScript : public CScript
{
private:
    CGameObject* m_Target;
    Vec4 m_BackGroundColor;
    bool m_bComplete;
    bool m_bReverse;
    float m_Duration;
    float m_ElapsedTime;
    float m_RotateSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    Vec4 GetBackGroundColor() const { return m_BackGroundColor; }
    void SetBackGroundColor(Vec4 _Color) { m_BackGroundColor = _Color; }

    bool IsComplete() const { return m_bComplete; }
    void ResetComplete() { m_bComplete = false; }

    bool IsReverse() const { return m_bReverse; }
    void SetReverse(bool _bReverse)
    {
        m_bReverse = _bReverse;
        ResetComplete();
    }

    float GetDuration() const { return m_Duration; }
    void SetDuration(float _Duration) { m_Duration = _Duration; }

    float GetRotateSpeed() const { return m_RotateSpeed; }
    void SetRotateSpeed(float _Speed) { m_RotateSpeed = _Speed; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CFadeOutScript);

public:
    CFadeOutScript();
    CFadeOutScript(const CFadeOutScript& origin);
    virtual ~CFadeOutScript();
};
