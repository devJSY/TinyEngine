#pragma once

#include <Engine/CScript.h>

enum class SmokeType
{
    Run,
    Spit
};

class CKirbySmoke : public CScript
{
private:
    UINT                        m_SmokeType;

    CGameObject*                m_SmokeOriginal;
    CGameObject*                m_SmokeSplit;
    CGameObject*                m_SmokeTail;

    float                       m_OriginalSize;
    float                       m_SplitSize;
    float                       m_TailSize;

    Vec3                        m_Dir;
    float                       m_Speed;

    float                       m_Acc;
    float                       m_MoveDuration;
    float                       m_FadeDuration;

public:
    void SetSpeed(float _Speed) { m_Speed = _Speed; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbySmoke);
    CKirbySmoke();
    CKirbySmoke(const CKirbySmoke& _Origin);
    virtual ~CKirbySmoke();
};
