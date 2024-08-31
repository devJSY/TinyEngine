#pragma once
#include <Engine\\CScript.h>

enum class ChangeAlphaEvent
{
    NONE,
    FadeIn,
    FadeIn_Random,
    FadeOut,
};

class CChangeAlphaScript : public CScript
{
private:
    SCALAR_PARAM m_AlphaParamIdx;
    vector<Ptr<CMaterial>> m_vecMtrl;
    ChangeAlphaEvent m_Event;

    float m_PlayTime;
    float m_MinPlayTime;
    vector<float> m_vecDelayTime;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

    void FadeIn(float _PlayTime) { SetEvent(ChangeAlphaEvent::FadeIn, _PlayTime); }
    void FadeIn_RandomDelay(float _MinPlayTime, float _MaxPlayTime);
    void FadeOutDestroy(float _PlayTime) { SetEvent(ChangeAlphaEvent::FadeOut, _PlayTime); }

public:
    void SetAlphaParamIdx(SCALAR_PARAM _Idx) { m_AlphaParamIdx = _Idx; }
    void SetAlpha(float _Alpha);
    void SetAlpha_Delay();
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
