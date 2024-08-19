#pragma once
#include <Engine\\CScript.h>

class CElfilisLaser : public CScript
{
private:
    StateStep m_Step;
    bool m_bAutoPlay;

public:
    virtual void tick() override;
    virtual void begin() override;

    void SetAutoPlay(bool _Play) { m_bAutoPlay = _Play; }
    void SetStart();
    void SetWait();
    void SetEnd();

public:
    StateStep GetStep() { return m_Step; }

private:
    void Start();
    void Progress();
    void End();

    void PlayOuter(const wstring _Anim, bool _bRepeat = true);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisLaser);
    CElfilisLaser();
    virtual ~CElfilisLaser();
};
