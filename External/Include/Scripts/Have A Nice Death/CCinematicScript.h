#pragma once
#include <Engine\\CScript.h>

class CCinematicScript : public CScript
{
protected:
    bool m_bTrigger;
    float m_ActionDuration;

    float m_PassedTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetEnableWallCollider(bool _bEnable);

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCinematicScript);

public:
    CCinematicScript();
    CCinematicScript(UINT _ScriptType);
    CCinematicScript(const CCinematicScript& origin);
    virtual ~CCinematicScript();
};
