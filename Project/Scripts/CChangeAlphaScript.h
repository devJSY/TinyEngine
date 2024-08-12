#pragma once
#include <Engine\\CScript.h>

enum class ChangeAlphaEvent
{
    NONE,
    FADE_IN,
    FADE_OUT,
};

class CChangeAlphaScript : public CScript
{
private:
    SCALAR_PARAM m_AlphaParamIdx;
    list<Ptr<CMaterial>> m_listMtrl;
    ChangeAlphaEvent m_Event;
    float m_PlayTime;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

    void FadeIn(float _PlayTime) { SetEvent(ChangeAlphaEvent::FADE_IN, _PlayTime); }
    void FadeOutDestroy(float _PlayTime) { SetEvent(ChangeAlphaEvent::FADE_OUT, _PlayTime); }

public:
    void SetAlphaParamIdx(SCALAR_PARAM _Idx) { m_AlphaParamIdx = _Idx; }
    void SetAlpha(float _Alpha);
    void SetEvent(ChangeAlphaEvent _Type, float _PlayTime);

private:
    void ClearEvent();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CChangeAlphaScript);
    CChangeAlphaScript();
    virtual ~CChangeAlphaScript();
};
